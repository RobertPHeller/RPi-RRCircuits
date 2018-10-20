#ifndef _APPLICATIONS_IO_BOARD_TARGET_CONFIG_HXX_
#define _APPLICATIONS_IO_BOARD_TARGET_CONFIG_HXX_

#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "openlcb/MemoryConfig.hxx"

#include "Turnout.hxx"
#include "Points.hxx"
#include "OccDetector.hxx"
#include "NoProducerOccDetector.hxx"
#include "Mast.hxx"
#include "ABSSlaveBus.hxx"

namespace openlcb
{

/// Defines the identification information for the node. The arguments are:
///
/// - 4 (version info, always 4 by the standard
/// - Manufacturer name
/// - Model name
/// - Hardware version
/// - Software version
///
/// This data will be used for all purposes of the identification:
///
/// - the generated cdi.xml will include this data
/// - the Simple Node Ident Info Protocol will return this data
/// - the ACDI memory space will contain this data.
extern const SimpleNodeStaticValues SNIP_STATIC_DATA = {
    4,               "Deepwoods Software", "RPiHalfSiding",
    "linux.armv7a", "1.00"};


/// Used for detecting when the config file stems from a different config.hxx
/// version and needs to be factory reset before using. Change every time that
/// the config eeprom file's layout changes.
static constexpr uint16_t CANONICAL_VERSION = 0x9000;

using TurnoutGroup = RepeatedGroup<TurnoutConfig, 2>;
using PointsGroup = RepeatedGroup<PointsConfig, 2>;

CDI_GROUP(MastGroup, Name("Masts"), Description("These are the signal masts"));
CDI_GROUP_ENTRY(points,MastPointsConfiguration,Name("Points Mast (3 over 2)"));
CDI_GROUP_ENTRY(frog_main,MastFrogConfiguration,Name("Frog Main Mast (3 over 1)"));
CDI_GROUP_ENTRY(frog_div,MastFrogConfiguration,Name("Frog Divergence Mast (1 over 3)"));
CDI_GROUP_END();

/// Defines the main segment in the configuration CDI. This is laid out at
/// origin 128 to give space for the ACDI user data at the beginning.
CDI_GROUP(IoBoardSegment, Name("RPiHalfSiding HAT"), Segment(MemoryConfigDefs::SPACE_CONFIG), Offset(128));
/// Each entry declares the name of the current entry, then the type and then
/// optional arguments list.
CDI_GROUP_ENTRY(internal_config, InternalConfigData);
CDI_GROUP_ENTRY(turnouts, TurnoutGroup, Name("Turnouts"), RepName("Turnout"));
CDI_GROUP_ENTRY(points, PointsGroup, Name("Points"), RepName("Points"));
CDI_GROUP_ENTRY(occupancy, OccupancyDetectorConfig, Name("OS Section"),  Description("Occupancy Detector for the OS Section"));
CDI_GROUP_ENTRY(eastpoints, NoProducerOccDetectorConfig, Name("East (Points)"), Description("Occupancy Detector for the block before the points."));
CDI_GROUP_ENTRY(westmain, NoProducerOccDetectorConfig, Name("West Main (Frog Normal)"), Description("Occupancy Detector for the block after the frog with points normal"));
CDI_GROUP_ENTRY(westdiverg, NoProducerOccDetectorConfig, Name("West Diverg (Frog Reversed)"), Description("Occupancy Detector for the block after the frog with the points reversed"));
CDI_GROUP_ENTRY(masts,MastGroup);
CDI_GROUP_ENTRY(abs_slave_list,ABSSlaveList,Name("ABS Slave Nodes"),RepName("Slave"));
CDI_GROUP_END();

/// This segment is only needed temporarily until there is program code to set
/// the ACDI user data version byte.
CDI_GROUP(VersionSeg, Segment(MemoryConfigDefs::SPACE_CONFIG),
    Name("Version information"));
CDI_GROUP_ENTRY(acdi_user_version, Uint8ConfigEntry,
    Name("ACDI User Data version"), Description("Set to 2 and do not change."));
CDI_GROUP_END();

/// The main structure of the CDI. ConfigDef is the symbol we use in main.cxx
/// to refer to the configuration defined here.
CDI_GROUP(ConfigDef, MainCdi());
/// Adds the <identification> tag with the values from SNIP_STATIC_DATA above.
CDI_GROUP_ENTRY(ident, Identification);
/// Adds an <acdi> tag.
CDI_GROUP_ENTRY(acdi, Acdi);
/// Adds a segment for changing the values in the ACDI user-defined
/// space. UserInfoSegment is defined in the system header.
CDI_GROUP_ENTRY(userinfo, UserInfoSegment, Name("User Info"));
/// Adds the main configuration segment.
CDI_GROUP_ENTRY(seg, IoBoardSegment);
/// Adds the versioning segment.
CDI_GROUP_ENTRY(version, VersionSeg);
CDI_GROUP_END();

} // namespace openlcb

#endif // _APPLICATIONS_IO_BOARD_TARGET_CONFIG_HXX_
