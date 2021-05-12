// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Apr 25 18:49:25 2021
//  Last Modified : <210512.1149>
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

#ifndef __BEAGLETRAINDATABASE_HXX
#define __BEAGLETRAINDATABASE_HXX

#include <vector>

#include <openlcb/Defs.hxx>
#include <openlcb/MemoryConfig.hxx>
#include <openlcb/SimpleInfoProtocol.hxx>
#include <openlcb/TractionTrain.hxx>
#include <os/OS.hxx>

#include <TrainDb.hxx>

#include <AutoPersistCallbackFlow.hxx>

#ifndef CONFIG_ROSTER_AUTO_IDLE_NEW_LOCOS
#define CONFIG_ROSTER_AUTO_IDLE_NEW_LOCOS false
#endif

namespace openlcb
{
    class SimpleStackBase;
}
    
namespace BeagleCS
{
    using namespace commandstation;

    struct BeaglePersistentTrainData 
    {
        uint16_t address;
        std::string name;
        std::string description;
        bool automatic_idle;
        bool show_on_limited_throttles;
        uint8_t mode;
        std::vector<uint8_t> functions;
        BeaglePersistentTrainData()
        {
        }
        BeaglePersistentTrainData(uint16_t address, 
                                  std::string name="unknown",
                                  std::string description="",
                                  DccMode mode=DccMode::DCC_128)
        {
            this->address = address;
            this->name = name;
            this->description = description;
            this->automatic_idle = CONFIG_ROSTER_AUTO_IDLE_NEW_LOCOS;
            this->show_on_limited_throttles = false;
            this->mode = mode;
            // set some defaults
            if (this->mode & DccMode::DCC_ANY)
            {
                this->functions.push_back(Symbols::LIGHT);
                this->functions.push_back(Symbols::BELL);
                this->functions.push_back(Symbols::HORN);
                while (this->functions.size() < DCC_MAX_FN)
                {
                    this->functions.push_back(Symbols::FN_UNKNOWN);
                }
            }
            else if (this->mode & DccMode::MARKLIN_ANY)
            {
                this->functions.push_back(Symbols::LIGHT);
                this->functions.push_back(Symbols::FN_UNKNOWN);
                this->functions.push_back(Symbols::FN_UNKNOWN);
                this->functions.push_back(Symbols::ABV);
                if (this->mode & DccMode::MARKLIN_TWOADDR)
                {
                    this->functions.push_back(Symbols::FN_UNKNOWN);
                    this->functions.push_back(Symbols::FN_UNKNOWN);
                    this->functions.push_back(Symbols::FN_UNKNOWN);
                    this->functions.push_back(Symbols::FN_UNKNOWN);
                }
            }
        }
    };

    class BeagleTrainDbEntry : public commandstation::TrainDbEntry
    {
    public:
        BeagleTrainDbEntry(BeaglePersistentTrainData, bool persist=true);
        std::string identifier() override;
        openlcb::NodeID get_traction_node() override;
        std::string get_train_name() override
        {
            return data_.name;
        }
        void set_train_name(std::string name)
        {
            data_.name = std::move(name);
            dirty_ = true;
        }
        std::string get_train_description() override
        {
            return data_.description;
        }
        void set_train_description(std::string description)
        {
            data_.description = std::move(description);
            dirty_ = true;
        }
        
        uint16_t get_legacy_address() override
        {
            return data_.address;
        }

        void set_legacy_address(uint16_t address)
        {
            data_.address = address;
            dirty_ = true;
        }

        DccMode get_legacy_drive_mode() override
        {
            return (DccMode)data_.mode;
        }

        void set_legacy_drive_mode(DccMode mode)
        {
            data_.mode = mode;
            dirty_ = true;
        }

        unsigned get_function_label(unsigned fn_id) override;

        void set_function_label(unsigned fn_id, Symbols label)
        {
            data_.functions[fn_id] = label;
            dirty_ = true;
            recalcuate_max_fn();
        }

        int get_max_fn() override
        {
            return maxFn_;
        }

        int file_offset() override;

        void start_read_functions() override
        {
        }

        BeaglePersistentTrainData get_data()
        {
            return data_;
        }
        
        void set_auto_idle(bool idle)
        {
            data_.automatic_idle = idle;
            dirty_ = true;
        }

        void set_show_on_limited_throttles(bool show)
        {
            data_.show_on_limited_throttles = show;
            dirty_ = true;
        }

        bool is_dirty()
        {
            return dirty_;
        }

        void reset_dirty(bool dirty=false)
        {
            dirty_ = dirty;
        }

        bool is_persisted()
        {
            return persist_;
        }

        bool is_auto_idle()
        {
            return data_.automatic_idle;
        }

        bool is_show_on_limited_throttles()
        {
            return data_.show_on_limited_throttles;
        }

    private:
        void recalcuate_max_fn();
        BeaglePersistentTrainData data_;
        uint8_t maxFn_;
        bool dirty_;
        bool persist_;
    };

    class BeagleTrainDatabase : public commandstation::TrainDb
              , public Singleton<BeagleTrainDatabase>
    {
    public:
        BeagleTrainDatabase(openlcb::SimpleStackBase *stack);
        
        void stop()
        {
            persistFlow_->stop();
        }

        // number of known trains
        size_t size() override
        {
            return knownTrains_.size();
        }

        int get_index(unsigned address);

        bool is_train_id_known(unsigned train_id) override
        {
            return get_entry(train_id) != nullptr;
        }

        bool is_train_id_known(openlcb::NodeID train_id) override;

        std::shared_ptr<commandstation::TrainDbEntry> 
              create_if_not_found(unsigned address, 
                                  std::string name="unknown",
                                  std::string description=""
                                  , DccMode mode=DccMode::DCC_128);

        void delete_entry(unsigned address);
        
        std::shared_ptr<commandstation::TrainDbEntry> get_entry(unsigned train_id) override;

        std::shared_ptr<commandstation::TrainDbEntry> 
              find_entry(openlcb::NodeID traction_node_id, 
                         unsigned hint = 0) override;

        unsigned add_dynamic_entry(uint16_t address, DccMode mode) 
              override;

        std::set<uint16_t> get_default_train_addresses(uint16_t limit);
        
        void set_train_name(unsigned address, std::string name);
        void set_train_description(unsigned address, std::string description);
        void set_train_auto_idle(unsigned address, bool idle);
        void set_train_show_on_limited_throttle(unsigned address, bool show);
        void set_train_function_label(unsigned address, uint8_t fn_id, Symbols label);
        void set_train_drive_mode(unsigned address, DccMode mode);

        std::string get_all_entries_as_json();
        std::string get_all_entries_as_list();
        std::string get_entry_as_json(unsigned address);
        DccMode get_train_mode(unsigned address);
        std::string get_train_name(unsigned address);
        std::string get_train_description(unsigned address);
        
        openlcb::MemorySpace *get_train_cdi()
        {
            return trainCdiFile_.get();
        }

        openlcb::MemorySpace *get_temp_train_cdi()
        {
            return tempTrainCdiFile_.get();
        }

        void persist();

    private:
        std::string get_entry_as_json_locked(unsigned address);
        openlcb::SimpleStackBase *stack_;
        bool entryDeleted_{false};
        OSMutex knownTrainsLock_;
        std::vector<std::shared_ptr<BeagleTrainDbEntry>> knownTrains_;
        std::unique_ptr<openlcb::MemorySpace> trainCdiFile_;
        std::unique_ptr<openlcb::MemorySpace> tempTrainCdiFile_;
        uninitialized<AutoPersistFlow> persistFlow_;
    };

extern const char *const TRAIN_DB_JSON_FILE;
#define CONFIG_ROSTER_PERSISTENCE_INTERVAL_SEC 10
} // namespace BeagleCS

#define CONFIG_ROSTER_AUTO_CREATE_ENTRIES

#endif // __BEAGLETRAINDATABASE_HXX

