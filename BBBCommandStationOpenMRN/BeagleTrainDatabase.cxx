// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Apr 25 21:14:31 2021
//  Last Modified : <210426.1913>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
//			51 Locke Hill Road
//			Wendell, MA 01379-9728
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// 
//
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include "BeagleTrainDatabase.hxx"

#include <AllTrainNodes.hxx>
#include <CDIHelper.hxx>
#include <json.hpp>
#include <JsonConstants.hxx>
#include <TrainDbCdi.hxx>
#include <utils/FileUtils.hxx>
#include <unistd.h>


#include <iostream>
#include <fstream>
#include <stdio.h>

#define exists(filename) (access(filename,F_OK) == 0)

static std::string load(const std::string& filename)
{
    std::ifstream is(filename, std::ios::ate);
    if (is.is_open()) {
        auto size = is.tellg();
        std::string str(size, '\0');
        is.seekg(0);
        if (is.read(&str[0], size)) {
            //LOG(INFO,"[load] str is '%s'",str.c_str());
            //LOG(INFO,"[load] str is %d bytes",str.size());
            return str;
        } else {
            return "";
        }
    } else {
        return "";
    }
}

static void store(const char * filename, const std::string& buffer)
{
    //LOG(INFO,"[store] buffer is '%s'",buffer.c_str());
    //LOG(INFO,"[store] buffer is %d bytes",buffer.size());
    std::ofstream(filename) << buffer << std::endl;
}


namespace commandstation
{
// JSON serialization mappings for the commandstation::DccMode enum
NLOHMANN_JSON_SERIALIZE_ENUM(DccMode,
{
  { DCCMODE_DEFAULT,     "DCC (default)" },
  { DCCMODE_OLCBUSER,    "DCC-OlcbUser" },
  { MARKLIN_DEFAULT,     "Marklin" },
  { MARKLIN_OLD,         "Marklin (v1)" },
  { MARKLIN_NEW,         "Marklin (v2, f0-f4)" },
  { MARKLIN_TWOADDR,     "Marklin (v2, f0-f8)" },
  { MFX,                 "Marklin (MFX)" },
  { DCC_DEFAULT,         "DCC (auto speed step)"},
  { DCC_14,              "DCC (14 speed step)"},
  { DCC_28,              "DCC (28 speed step)"},
  { DCC_128,             "DCC (128 speed step)"},
  { DCC_14_LONG_ADDRESS, "DCC (14 speed step, long address)"},
  { DCC_28_LONG_ADDRESS, "DCC (28 speed step, long address)"},
  { DCC_128_LONG_ADDRESS,"DCC (128 speed step, long address)"},
});

// JSON serialization mappings for the commandstation::Symbols enum
NLOHMANN_JSON_SERIALIZE_ENUM(Symbols,
{
  { FN_NONEXISTANT,   "N/A" },
  { LIGHT,            "Light" },
  { BEAMER,           "Beamer" },
  { BELL,             "Bell" },
  { HORN,             "Horn" },
  { SHUNT,            "Shunting mode" },
  { PANTO,            "Pantograph" },
  { SMOKE,            "Smoke" },
  { ABV,              "Momentum On/Off" },
  { WHISTLE,          "Whistle" },
  { SOUND,            "Sound" },
  { FNT11,            "Generic Function" },
  { SPEECH,           "Announce" },
  { ENGINE,           "Engine" },
  { LIGHT1,           "Light1" },
  { LIGHT2,           "Light2" },
  { TELEX,            "Coupler" },
  { FN_UNKNOWN,       "Unknown" },
  { MOMENTARY,        "momentary" },
  { FNP,              "fnp" },
  { SOUNDP,           "soundp" },
  { FN_UNINITIALIZED, "uninit" },
})
}

namespace BeagleCS
{
using nlohmann::json;
using commandstation::DccMode;
using commandstation::AllTrainNodes;
using openlcb::TractionDefs;

static constexpr char TRAIN_CDI_FILE[] = "/tmp/train.xml";
static constexpr char TEMP_TRAIN_CDI_FILE[] = "/tmp/tmptrain.xml";

// This should really be defined inside TractionDefs.hxx and used by the call
// to TractionDefs::train_node_id_from_legacy().
static constexpr uint64_t const OLCB_NODE_ID_USER = 0x050101010000ULL;

// converts a BeaglePersistentTrainData to a json object
void to_json(json& j, const BeaglePersistentTrainData& t)
{
    //LOG(INFO,"[to_json] Converting to JSON: t.mode is 0x%0x",(unsigned)t.mode);
    j = json({
         { JSON_NAME_NODE, t.name },
         { JSON_DESCRIPTION_NODE, t.description },
         { JSON_ADDRESS_NODE, t.address },
         { JSON_IDLE_ON_STARTUP_NODE, t.automatic_idle },
         { JSON_DEFAULT_ON_THROTTLE_NODE, t.show_on_limited_throttles },
         { JSON_FUNCTIONS_NODE, t.functions },
         { JSON_MODE_NODE, t.mode },
         });
    //LOG(INFO,"[to_json] Converted to JSON: j is '%s'",j.dump().c_str());
}

// converts a json payload to a BeaglePersistentTrainData object
void from_json(const json& j, BeaglePersistentTrainData& t)
{
    j.at(JSON_NAME_NODE).get_to(t.name);
    j.at(JSON_DESCRIPTION_NODE).get_to(t.description);
    j.at(JSON_ADDRESS_NODE).get_to(t.address);
    j.at(JSON_IDLE_ON_STARTUP_NODE).get_to(t.automatic_idle);
    j.at(JSON_DEFAULT_ON_THROTTLE_NODE).get_to(t.show_on_limited_throttles);
    j.at(JSON_FUNCTIONS_NODE).get_to(t.functions);
    j.at(JSON_MODE_NODE).get_to(t.mode);
}

BeagleTrainDbEntry::BeagleTrainDbEntry(BeaglePersistentTrainData data
                                       , bool persist)
      : data_(data), dirty_(true), persist_(persist)
{
    recalcuate_max_fn();
    LOG(VERBOSE, "[Loco:%s] Locomotive '%s' created", identifier().c_str()
        , data_.name.c_str());
}

string BeagleTrainDbEntry::identifier()
{
      dcc::TrainAddressType addrType =
          dcc_mode_to_address_type((DccMode)data_.mode, data_.address);
    if (addrType == dcc::TrainAddressType::DCC_SHORT_ADDRESS ||
        addrType == dcc::TrainAddressType::DCC_LONG_ADDRESS)
    {
        string prefix = "long_address";
        if (addrType == dcc::TrainAddressType::DCC_SHORT_ADDRESS)
        {
            prefix = "short_address";
        }
        if ((data_.mode & DCC_SS_MASK) == 1)
        {
            return StringPrintf("dcc_14/%s/%d", prefix.c_str(), data_.address);
        }
        else if ((data_.mode & DCC_SS_MASK) == 2)
        {
            return StringPrintf("dcc_28/%s/%d", prefix.c_str(), data_.address);
        }
        else
        {
            return StringPrintf("dcc_128/%s/%d", prefix.c_str(), data_.address);
        }
    }
    else if (addrType == dcc::TrainAddressType::MM)
    {
        // TBD: should marklin types be explored further?
        return StringPrintf("marklin/%d", data_.address);
    }
    return StringPrintf("unknown/%d", data_.address);
}

openlcb::NodeID BeagleTrainDbEntry::get_traction_node()
{
    if (data_.mode == DCCMODE_OLCBUSER)
    {
        return OLCB_NODE_ID_USER | static_cast<openlcb::NodeID>(data_.address);
    }
    else
    {
        return openlcb::TractionDefs::train_node_id_from_legacy(
                                                                dcc_mode_to_address_type((DccMode)data_.mode, data_.address), data_.address);
    }
}

unsigned BeagleTrainDbEntry::get_function_label(unsigned fn_id)
{
    // if the function id is larger than our max list reject it
    if (fn_id > maxFn_)
    {
        return FN_NONEXISTANT;
    }
    // return the mapping for the function
    return data_.functions[fn_id];
}

int BeagleTrainDbEntry::file_offset()
{
    if (persist_)
    {
        return Singleton<BeagleTrainDatabase>::instance()->get_index(data_.address);
    }
    // non-persistent entries should not have an offset
    return -1;
}

void BeagleTrainDbEntry::recalcuate_max_fn()
{
    // recalculate the maxFn_ based on the first occurrence of FN_NONEXISTANT
    // and if not found default to the size of the functions labels vector.
    auto e = std::find_if(data_.functions.begin(), data_.functions.end()
                          , [](const uint8_t &e)
                      {
                          return e == FN_NONEXISTANT;
                      });
    
    if (e != data_.functions.end())
    {
        maxFn_ = std::distance(data_.functions.begin(), e);
    }
    else
    {
        maxFn_ = data_.functions.size();
    }
}

BeagleTrainDatabase::BeagleTrainDatabase(openlcb::SimpleStackBase *stack)
{
    TrainConfigDef trainCfg(0);
    TrainTmpConfigDef tmpTrainCfg(0);
    CDIHelper::create_config_descriptor_xml(trainCfg, TRAIN_CDI_FILE);
    CDIHelper::create_config_descriptor_xml(tmpTrainCfg, TEMP_TRAIN_CDI_FILE);
    trainCdiFile_.reset(new openlcb::ROFileMemorySpace(TRAIN_CDI_FILE));
    tempTrainCdiFile_.reset(new openlcb::ROFileMemorySpace(TEMP_TRAIN_CDI_FILE));
    persistFlow_.emplace(stack->service()
                         , SEC_TO_NSEC(CONFIG_ROSTER_PERSISTENCE_INTERVAL_SEC)
                         , std::bind(&BeagleTrainDatabase::persist, this));
}

void BeagleTrainDatabase::Begin()
{
    LOG(INFO, "[TrainDB] Initializing...");
    if (exists(TRAIN_DB_JSON_FILE))
    {
        auto roster = load(TRAIN_DB_JSON_FILE);
        json stored_trains = json::parse(roster, nullptr, false);
        if (stored_trains.is_discarded())
        {
            LOG_ERROR("[TrainDB] database is corrupt, no trains loaded!");
            remove(TRAIN_DB_JSON_FILE);
            return;
        }
        for (auto &entry : stored_trains)
        {
            auto data = entry.get<BeaglePersistentTrainData>();
            auto ent = std::find_if(knownTrains_.begin(), knownTrains_.end(),
                                    [data](const std::shared_ptr<BeagleTrainDbEntry> &train)
                                {
                                    return train->get_legacy_address() == data.address;
                                });
            if (ent == knownTrains_.end())
            {
                LOG(INFO, "[TrainDB] Registering %u - %s (idle: %s, limited: %s)"
                    , data.address, data.name.c_str()
                    , data.automatic_idle ? JSON_VALUE_ON : JSON_VALUE_OFF
                    , data.show_on_limited_throttles ? JSON_VALUE_ON : JSON_VALUE_OFF);
                auto train = new BeagleTrainDbEntry(data);
                train->reset_dirty();
                if (train->is_auto_idle())
                {
                    uint16_t address = train->get_legacy_address();
                    stack_->executor()->add(new CallbackExecutable([address]()
                                                              {
                                                                  auto trainMgr = Singleton<AllTrainNodes>::instance();
                                                                  trainMgr->allocate_node(DccMode::DCC_128, address);
                                                              }));
                }
                knownTrains_.emplace_back(train);
            }
            else
            {
                LOG_ERROR("[TrainDB] Duplicate roster entry detected for loco addr %u."
                          , data.address);
            }
        }
    }
    
    LOG(INFO, "[TrainDB] Found %d persistent roster entries."
        , knownTrains_.size());
}



#define FIND_TRAIN(id)                                    \
  std::find_if(knownTrains_.begin(), knownTrains_.end(),  \
    [id](const std::shared_ptr<BeagleTrainDbEntry> &train)                               \
    {                                                     \
      return train->get_legacy_address() == id;           \
    })

#define FIND_TRAIN_HINT(id, id2)                          \
  std::find_if(knownTrains_.begin(), knownTrains_.end(),  \
    [id,id2](const std::shared_ptr<BeagleTrainDbEntry> &train)                           \
    {                                                     \
      return train->get_traction_node() == id ||          \
             train->get_legacy_address() == id2;          \
    })

std::shared_ptr<TrainDbEntry> BeagleTrainDatabase::create_if_not_found(unsigned address
                                                                    , string name, string description
                                                                    , DccMode mode)
{
  OSMutexLock l(&knownTrainsLock_);
  LOG(VERBOSE, "[TrainDB] Searching for roster entry for address: %u", address);
  auto entry = FIND_TRAIN(address);
  if (entry != knownTrains_.end())
  {
    LOG(VERBOSE, "[TrainDB] Found existing entry:%s."
      , (*entry)->identifier().c_str());
    return *entry;
  }
  auto index = knownTrains_.size();
  knownTrains_.emplace_back(
    new BeagleTrainDbEntry(BeaglePersistentTrainData(address, name, description, mode)));
  LOG(VERBOSE, "[TrainDB] No entry was found, created new entry:%s."
    , knownTrains_[index]->identifier().c_str());
  return knownTrains_[index];
}

int BeagleTrainDatabase::get_index(unsigned address)
{
  auto ent = FIND_TRAIN(address);
  if (ent != knownTrains_.end())
  {
    return std::distance(knownTrains_.begin(), ent);
  }

  return -1;
}

bool BeagleTrainDatabase::is_train_id_known(openlcb::NodeID train_id)
{
  LOG(VERBOSE, "[TrainDB] searching for train with id: %s"
    , uint64_to_string_hex(train_id).c_str());
  dcc::TrainAddressType type;
  uint32_t addr =  0;
  // verify that it is a valid train node id
  if (TractionDefs::legacy_address_from_train_node_id(train_id, &type, &addr))
  {
    // only search with the address and discard the drive type (for now)
    auto ent = FIND_TRAIN(addr);
    if (ent != knownTrains_.end())
    {
      LOG(VERBOSE, "[TrainDB] %s", (*ent)->identifier().c_str());
    }
    return ent != knownTrains_.end();
  }
  return false;
}

void BeagleTrainDatabase::delete_entry(unsigned address)
{
  OSMutexLock l(&knownTrainsLock_);
  auto entry = FIND_TRAIN(address);
  if (entry != knownTrains_.end())
  {
    LOG(VERBOSE, "[TrainDB] Removing persistent entry for address %u", address);
    knownTrains_.erase(entry);
    entryDeleted_ = true;
  }
}

std::shared_ptr<TrainDbEntry> BeagleTrainDatabase::get_entry(unsigned train_id)
{
  OSMutexLock l(&knownTrainsLock_);
  LOG(VERBOSE, "[TrainDB] get_entry(%u) : %zu", train_id, knownTrains_.size());
  if (train_id < knownTrains_.size())
  {
    return knownTrains_[train_id];
  }
  // check if the train_id is a locomotive address that we know of
  auto entry = FIND_TRAIN(train_id);
  if (entry != knownTrains_.end())
  {
    return *entry;
  }
  return nullptr;
}

std::shared_ptr<TrainDbEntry> BeagleTrainDatabase::find_entry(openlcb::NodeID node_id
                                                           , unsigned hint)
{
  OSMutexLock l(&knownTrainsLock_);
  LOG(VERBOSE, "[TrainDB] Searching for Train Node:%s, Hint:%u"
    , uint64_to_string(node_id).c_str(), hint);
  auto entry = FIND_TRAIN_HINT(node_id, hint);
  if (entry != knownTrains_.end())
  {
    LOG(VERBOSE, "[TrainDB] Found existing entry: %s."
      , (*entry)->identifier().c_str());
    return *entry;
  }
  LOG(VERBOSE, "[TrainDB] No entry found!");
  return nullptr;
}

// The caller of this method expects a zero based index into the vector. This
// may be changed in the future to use the loco address instead.
unsigned BeagleTrainDatabase::add_dynamic_entry(uint16_t address, DccMode mode)
{
  OSMutexLock l(&knownTrainsLock_);
  size_t index = 0;
  
  LOG(VERBOSE, "[TrainDB] Searching for loco %d", address);

  // prevent duplicate entries in the roster
  auto ent = FIND_TRAIN(address);
  if (ent != knownTrains_.end())
  {
    index = std::distance(knownTrains_.begin(), ent);
    LOG(VERBOSE, "[TrainDB] Found existing entry (%zu)", index);
  }
  else
  {
    // track the index for the new train entry
    index = knownTrains_.size();

#ifdef CONFIG_ROSTER_AUTO_CREATE_ENTRIES
    LOG(VERBOSE
      , "[TrainDB] Creating persistent roster entry for locomotive %u."
      , address);

    // create the new entry, it will default to being marked dirty so it will
    // automatically persist.
    knownTrains_.emplace_back(
      new BeagleTrainDbEntry(
        BeaglePersistentTrainData(address, std::to_string(address), "", mode)));
#else
    LOG(VERBOSE
      , "[TrainDB] Adding temporary roster entry for locomotive %u."
      , address);
    // create the new entry and do not mark it as dirty so it doesn't
    // automatically persist. If the locomotive is later edited via the web UI
    // it will be marked as dirty and persisted at that point.
    knownTrains_.emplace_back(
      new BeagleTrainDbEntry(
        BeaglePersistentTrainData(address, std::to_string(address), "", mode)
      , false));
#endif
  }
  return index;
}

std::set<uint16_t> BeagleTrainDatabase::get_default_train_addresses(uint16_t limit)
{
  std::set<uint16_t> results;
  OSMutexLock l(&knownTrainsLock_);
  for(auto entry : knownTrains_)
  {
    if (entry->get_data().show_on_limited_throttles)
    {
      if (limit)
      {
        results.insert(entry->get_legacy_address());
        limit--;
      }
      else
      {
        break;
      }
    }
  }
  return results;
}

void BeagleTrainDatabase::set_train_name(unsigned address, std::string name)
{
  OSMutexLock l(&knownTrainsLock_);
  LOG(VERBOSE, "[TrainDB] Searching for train with address %u", address);
  auto entry = FIND_TRAIN(address);
  if (entry != knownTrains_.end())
  {
    LOG(VERBOSE, "[TrainDB] Setting train(%u) name: %s", address, name.c_str());
    (*entry)->set_train_name(name);
  }
  else
  {
    LOG_ERROR("[TrainDB] train %u not found, unable to set the name!", address);
  }
}

void BeagleTrainDatabase::set_train_description(unsigned address, std::string description)
{
  OSMutexLock l(&knownTrainsLock_);
  LOG(VERBOSE, "[TrainDB] Searching for train with address %u", address);
  auto entry = FIND_TRAIN(address);
  if (entry != knownTrains_.end())
  {
    LOG(VERBOSE, "[TrainDB] Setting train(%u) description: %s", address, description.c_str());
    (*entry)->set_train_description(description);
  }
  else
  {
    LOG_ERROR("[TrainDB] train %u not found, unable to set the description!", address);
  }
}

void BeagleTrainDatabase::set_train_auto_idle(unsigned address, bool idle)
{
  OSMutexLock l(&knownTrainsLock_);
  LOG(VERBOSE, "[TrainDB] Searching for train with address %u", address);
  auto entry = FIND_TRAIN(address);
  if (entry != knownTrains_.end())
  {
    LOG(VERBOSE, "[TrainDB] Setting auto-idle: %s"
      , idle ? JSON_VALUE_ON : JSON_VALUE_OFF);
    (*entry)->set_auto_idle(idle);
  }
  else
  {
    LOG_ERROR("[TrainDB] train %u not found, unable to set idle state!"
            , address);
  }
}

void BeagleTrainDatabase::set_train_show_on_limited_throttle(unsigned address
                                                          , bool show)
{
  OSMutexLock l(&knownTrainsLock_);
  LOG(VERBOSE, "[TrainDB] Searching for train with address %u", address);
  auto entry = FIND_TRAIN(address);
  if (entry != knownTrains_.end())
  {
    LOG(VERBOSE, "[TrainDB] Setting visible on limited throttes: %s"
      , show ? JSON_VALUE_ON : JSON_VALUE_OFF);
    (*entry)->set_show_on_limited_throttles(show);
  }
  else
  {
    LOG_ERROR("[TrainDB] train %u not found, unable to set limited throttle!"
            , address);
  }
}

void BeagleTrainDatabase::set_train_function_label(unsigned address, uint8_t fn_id, Symbols label)
{
  OSMutexLock l(&knownTrainsLock_);
  LOG(VERBOSE, "[TrainDB] Searching for train with address %u", address);
  auto entry = FIND_TRAIN(address);
  if (entry != knownTrains_.end())
  {
    (*entry)->set_function_label(fn_id, label);
  }
  else
  {
    LOG_ERROR("[TrainDB] train %u not found, unable to set function label!"
            , address);
  }
}

void BeagleTrainDatabase::set_train_drive_mode(unsigned address, commandstation::DccMode mode)
{
  OSMutexLock l(&knownTrainsLock_);
  LOG(VERBOSE, "[TrainDB] Searching for train with address %u", address);
  auto entry = FIND_TRAIN(address);
  if (entry != knownTrains_.end())
  {
    (*entry)->set_legacy_drive_mode(mode);
  }
  else
  {
    LOG_ERROR("[TrainDB] train %u not found, unable to set drive mode!"
            , address);
  }
}

string BeagleTrainDatabase::get_all_entries_as_json()
{
  OSMutexLock l(&knownTrainsLock_);
  string res = "[";
  for (auto entry : knownTrains_)
  {
    if (res.length() > 1)
    {
      res += ",";
    }
    res += get_entry_as_json_locked(entry->get_legacy_address());
  }
  res += "]";
  return res;
}

string BeagleTrainDatabase::get_all_entries_as_list()
{
    OSMutexLock l(&knownTrainsLock_); 
    string res = "";
    for (auto entry : knownTrains_)
    {
        if (res.length() > 0)
        {
            res += " ";
        }
        res += std::to_string(entry->get_legacy_address());
    }
    return res;
}

string BeagleTrainDatabase::get_entry_as_json(unsigned address)
{
  OSMutexLock l(&knownTrainsLock_);
  return get_entry_as_json_locked(address);
}

DccMode BeagleTrainDatabase::get_train_mode(unsigned address)
{
    OSMutexLock l(&knownTrainsLock_);
    auto entry = FIND_TRAIN(address);
    if (entry != knownTrains_.end())
    {
        auto train = (*entry);
        return train->get_legacy_drive_mode();
    } else {
        return DccMode::DCCMODE_DEFAULT;
    }
}
string BeagleTrainDatabase::get_train_name(unsigned address)
{
    OSMutexLock l(&knownTrainsLock_);
    auto entry = FIND_TRAIN(address);
    if (entry != knownTrains_.end())
    {
        auto train = (*entry);
        return train->get_train_name();
    } else {
        return "";
    }
}

string BeagleTrainDatabase::get_train_description(unsigned address)
{
    OSMutexLock l(&knownTrainsLock_);
    auto entry = FIND_TRAIN(address);
    if (entry != knownTrains_.end())
    {
        auto train = (*entry);
        return train->get_train_description();
    } else {
        return "";
    }
}


string BeagleTrainDatabase::get_entry_as_json_locked(unsigned address)
{
  auto entry = FIND_TRAIN(address);
  if (entry != knownTrains_.end())
  {
    auto train = (*entry);
    json j =
    {
      { "name", train->get_train_name() },
      { "address", train->get_legacy_address() },
      { "idleOnStartup", train->is_auto_idle() },
      { "defaultOnThrottles", train->is_show_on_limited_throttles() },
      { "mode",
        {
          { "name", static_cast<DccMode>(train->get_legacy_drive_mode()) },
          { "type", train->get_legacy_drive_mode() }
        }
      }
    };
    for (uint8_t idx = 0; idx < DCC_MAX_FN; idx++)
    {
      j["functions"].push_back(
      {
        { "id", idx },
        { "name", static_cast<Symbols>(train->get_function_label(idx)) },
        { "type", train->get_function_label(idx) }
      });
    }
    return j.dump();
  }
  return "{}";
}

void BeagleTrainDatabase::persist()
{
  OSMutexLock l(&knownTrainsLock_);
  LOG(VERBOSE, "[TrainDB] Checking if roster needs to be persisted...");
  auto ent = std::find_if(knownTrains_.begin(), knownTrains_.end(),
    [](const std::shared_ptr<BeagleTrainDbEntry> &train)
    {
      return train->is_dirty() && train->is_persisted();
    });
  if (ent != knownTrains_.end() || entryDeleted_)
  {
    LOG(VERBOSE, "[TrainDB] At least one entry requires persistence.");
    json j;
    size_t count = 0;
    for (auto entry : knownTrains_)
    {
      if (entry->is_persisted())
      {
        j.push_back(entry->get_data());
        count++;
      }
      entry->reset_dirty();
    }
    store(TRAIN_DB_JSON_FILE, j.dump());
    LOG(INFO, "[TrainDB] Persisted %zu entries.", count);
    entryDeleted_ = false;
  }
  else
  {
    LOG(VERBOSE, "[TrainDB] No entries require persistence");
  }
}

} // namespace BeagleCS
