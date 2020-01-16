#ifndef _PRINTER_ALERT_DEF_F1C295E9_F82E_4E43_914E_D7F0E780854A_H_
#define _PRINTER_ALERT_DEF_F1C295E9_F82E_4E43_914E_D7F0E780854A_H_

#include "Snmp_Def.h"

// 预警类别,兼容【PrinterMIBv2_Def.h】中数据枚举的【PrtAlertGroupTC】
BEGIN_ENUM_STRUCT(PrinterAlertType)
{
	Other = 1,
	// (2) is reserved for conformance information
	// Values for Host Resources MIB
	HostResourcesMIBStorageTable = 3,
	HostResourcesMIBDeviceTable = 4,
	// Values for Printer MIB
	GeneralPrinter = 5,
	Cover = 6,			//盖板
	Localization = 7,
	Input = 8,
	Output = 9,
	Marker = 10,
	MarkerSupplies = 11,
	MarkerColorant = 12,
	MediaPath = 13,
	Channel = 14,
	Interpreter = 15,
	ConsoleDisplayBuffer = 16,
	ConsoleLights = 17,
	Alert = 18,                   // Not in RFC 1759
	// Values (5) to (29) reserved for Printer MIB
	// Values for Finisher MIB
	FinDevice = 30,               // Not in RFC 1759
	FinSupply = 31,               // Not in RFC 1759
	FinSupplyMediaInput = 32,     // Not in RFC 1759
	FinAttribute = 33,            // Not in RFC 1759
	// Values (30) to (39) reserved for Finisher MIB

	//以下是信安宝云打印预警扩展,从(50)以后开始
	Drive = 50,		//打印驱动程序的预警
	NetWork = 51,	//打印与主机的网络环境
	MFPControlHardware = 52,	//MFP的硬件控制组件，如复印控制器，电源控制器，arm硬件系统
	Tary = 53,		//纸盒
	Toner = 54,		//粉盒
	Ink = 55,		//墨盒
	OPC = 56,		//感光鼓
	Developer = 57,	//显影单元
	PrinterMonitor = 58,	//打印机状态监视
	PrinterFault = 59 //打印机故障

}
END_ENUM_STRUCT(PrinterAlertType)

// 预警码,兼容【PrinterMIBv2_Def.h】中数据枚举的【PrtAlertCodeTC】
 BEGIN_ENUM_STRUCT(PrinterAlertCode)
{
	Other = 1,
	// an event that is not represented
	// by one of the alert codes
	// specified below.
	UnKnown = 2,
	// The following generic codes are common to
	// multiple groups.  The NMS may examine the
	// prtAlertGroup object to determine what group
	// to query for further information.
	CoverOpen = 3,
	CoverClosed = 4,
	InterlockOpen = 5,
	InterlockClosed = 6,
	ConfigurationChange = 7,
	Jam = 8,
	SubunitMissing = 9,           // Not in RFC 1759
	// The subunit tray, bin, etc.
	// has been removed.
	SubunitLifeAlmostOver = 10,   // Not in RFC 1759
	SubunitLifeOver = 11,         // Not in RFC 1759
	SubunitAlmostEmpty = 12,      // Not in RFC 1759
	SubunitEmpty = 13,            // Not in RFC 1759
	SubunitAlmostFull = 14,       // Not in RFC 1759
	SubunitFull = 15,             // Not in RFC 1759
	SubunitNearLimit = 16,        // Not in RFC 1759
	SubunitAtLimit = 17,          // Not in RFC 1759
	SubunitOpened = 18,           // Not in RFC 1759
	SubunitClosed = 19,           // Not in RFC 1759
	SubunitTurnedOn = 20,         // Not in RFC 1759
	SubunitTurnedOff = 21,        // Not in RFC 1759
	SubunitOffline = 22,          // Not in RFC 1759
	SubunitPowerSaver = 23,       // Not in RFC 1759
	SubunitWarmingUp = 24,        // Not in RFC 1759
	SubunitAdded = 25,            // Not in RFC 1759
	SubunitRemoved = 26,          // Not in RFC 1759
	SubunitResourceAdded = 27,    // Not in RFC 1759
	SubunitResourceRemoved = 28,  // Not in RFC 1759
	SubunitRecoverableFailure = 29,
	// Not in RFC 1759
	SubunitUnrecoverableFailure = 30,
	// Not in RFC 1759
	SubunitRecoverableStorageError = 31,
	// Not in RFC 1759
	SubunitUnrecoverableStorageError = 32,
	// Not in RFC 1759
	SubunitMotorFailure = 33,     // Not in RFC 1759
	SubunitMemoryExhausted = 34,  // Not in RFC 1759
	SubunitUnderTemperature = 35, // Not in RFC 1759
	SubunitOverTemperature = 36,  // Not in RFC 1759
	SubunitTimingFailure = 37,    // Not in RFC 1759
	SubunitThermistorFailure = 38, // Not in RFC 1759

	//信安宝扩展多组预警，begin
	MultipleGroupsOther = 101,
	MultipleGroupsUnKnown = 102,
	MultipleGroupsCoverOpen = 103,
	MultipleGroupsCoverClosed = 104,
	MultipleGroupsInterlockOpen = 105,
	MultipleGroupsInterlockClosed = 106,
	MultipleGroupsConfigurationChange = 107,
	MultipleGroupsJam = 108,
	MultipleGroupsSubunitMissing = 109,
	MultipleGroupsSubunitLifeAlmostOver = 10,
	MultipleGroupsSubunitLifeOver = 111,
	MultipleGroupsSubunitAlmostEmpty = 112,
	MultipleGroupsSubunitEmpty = 113,
	MultipleGroupsSubunitAlmostFull = 114,
	MultipleGroupsSubunitFull = 115,
	MultipleGroupsSubunitNearLimit = 116,
	MultipleGroupsSubunitAtLimit = 117,
	MultipleGroupsSubunitOpened = 118,
	MultipleGroupsSubunitClosed = 119,
	MultipleGroupsSubunitTurnedOn = 120,
	MultipleGroupsSubunitTurnedOff = 121,
	MultipleGroupsSubunitOffline = 122,
	MultipleGroupsSubunitPowerSaver = 123,
	MultipleGroupsSubunitWarmingUp = 124,
	MultipleGroupsSubunitAdded = 125,
	MultipleGroupsSubunitRemoved = 126,
	MultipleGroupsSubunitResourceAdded = 127,
	MultipleGroupsSubunitResourceRemoved = 128,
	MultipleGroupsSubunitRecoverableFailure = 129,
	MultipleGroupsSubunitUnrecoverableFailure = 130,
	MultipleGroupsSubunitRecoverableStorageError = 131,
	MultipleGroupsSubunitUnrecoverableStorageError = 132,
	MultipleGroupsSubunitMotorFailure = 133,
	MultipleGroupsSubunitMemoryExhausted = 134,
	MultipleGroupsSubunitUnderTemperature = 135,
	MultipleGroupsSubunitOverTemperature = 136,
	MultipleGroupsSubunitTimingFailure = 137,
	MultipleGroupsSubunitThermistorFailure = 138,
	//信安宝扩展多组预警，begin

	// General Printer group
	DoorOpen = 501,    // DEPRECATED
	// Use coverOpened(3)
	DoorClosed = 502,  // DEPRECATED
	// Use coverClosed(4)
	PowerUp = 503,
	PowerDown = 504,
	PrinterNMSReset = 505,        // Not in RFC 1759
	// The printer has been reset by some
	// network management station(NMS)
	// writing into 'prtGeneralReset'.
	PrinterManualReset = 506,     // Not in RFC 1759
	// The printer has been reset manually.
	PrinterReadyToPrint = 507,    // Not in RFC 1759
	// The printer is ready to print. (i.e.,
	// not warming up, not in power save
	// state, not adjusting print quality,
	// etc.).

	// Input Group
	InputMediaTrayMissing = 801,
	InputMediaSizeChange = 802,
	InputMediaWeightChange = 803,
	InputMediaTypeChange = 804,
	InputMediaColorChange = 805,
	InputMediaFormPartsChange = 806,
	InputMediaSupplyLow = 807,
	InputMediaSupplyEmpty = 808,
	InputMediaChangeRequest = 809, // Not in RFC 1759
	// An interpreter has detected that a
	// different medium is need in this input
	// tray subunit.  The prtAlertDescription may
	// be used to convey a human readable
	// description of the medium required to
	// satisfy the request.
	InputManualInputRequest = 810, // Not in RFC 1759
	// An interpreter has detected that manual
	// input is required in this subunit.  The
	// prtAlertDescription may be used to convey
	// a human readable description of the medium
	// required to satisfy the request.
	InputTrayPositionFailure = 811, // Not in RFC 1759
	// The input tray failed to position correctly.
	InputTrayElevationFailure = 812,
	// Not in RFC 1759
	InputCannotFeedSizeSelected = 813,
	// Not in RFC 1759
	// Output Group
	OutputMediaTrayMissing = 901,
	OutputMediaTrayAlmostFull = 902,
	OutputMediaTrayFull = 903,
	OutputMailboxSelectFailure = 904,
	// Not in RFC 1759
	// Marker group
	MarkerFuserUnderTemperature = 1001,
	MarkerFuserOverTemperature = 1002,
	MarkerFuserTimingFailure = 1003,
	// Not in RFC 1759
	MarkerFuserThermistorFailure = 1004,
	// Not in RFC 1759
	MarkerAdjustingPrintQuality = 1005,
	// Not in RFC 1759
	// Marker Supplies group
	MarkerTonerEmpty = 1101,
	MarkerInkEmpty = 1102,
	MarkerPrintRibbonEmpty = 1103,
	MarkerTonerAlmostEmpty = 1104,
	MarkerInkAlmostEmpty = 1105,
	MarkerPrintRibbonAlmostEmpty = 1106,
	MarkerWasteTonerReceptacleAlmostFull = 1107,
	MarkerWasteInkReceptacleAlmostFull = 1108,
	MarkerWasteTonerReceptacleFull = 1109,
	MarkerWasteInkReceptacleFull = 1110,
	MarkerOpcLifeAlmostOver = 1111,
	MarkerOpcLifeOver = 1112,
	MarkerDeveloperAlmostEmpty = 1113,
	MarkerDeveloperEmpty = 1114,
	MarkerTonerCartridgeMissing = 1115,
	// Not in RFC 1759
	// Media Path Device Group
	MediaPathMediaTrayMissing = 1301,
	MediaPathMediaTrayAlmostFull = 1302,
	MediaPathMediaTrayFull = 1303,
	MediaPathCannotDuplexMediaSelected = 1304,
	// Not in RFC 1759
	// Interpreter Group
	InterpreterMemoryIncrease = 1501,
	InterpreterMemoryDecrease = 1502,
	InterpreterCartridgeAdded = 1503,
	InterpreterCartridgeDeleted = 1504,
	InterpreterResourceAdded = 1505,
	InterpreterResourceDeleted = 1506,
	InterpreterResourceUnavailable = 1507,
	InterpreterComplexPageEncountered = 1509,
	//add by zxl,控制面板显示（16）
	ConsoleDisplayBufferText = 1601,
	// Not in RFC 1759
	// The interpreter has encountered a page
	// that is too complex for the resources that
	// are available.
	// Alert Group
	AlertRemovalOfBinaryChangeEntry = 1801,
	// Not in RFC 1759
	// A binary change event entry has been
	// removed from the alert table.  This unary
	// change alert table entry is added to the
	// end of the alert table.

	//以下是信安宝云打印预警扩展,50以后
	//驱动程序的预警(50)，参考WinSpool.h PRINTER_STATUS_*
	DrivePaused = 5001,
	DriveError = 5002,
	DrivePendingDeletion = 5003,
	DrivePaperJam = 5004,
	DrivePaperOut = 5005,
	DriveManualFeed = 5006,
	DrivePaperProblem = 5007,
	DriveOffline = 5008,
	DriveIOActive = 5009,
	DriveBusy = 5010,
	DrivePrinting = 5011,
	DriveOutputBinFull = 5012,
	DriveNotAvailable = 5013,
	DriveWaiting = 5014,
	DriveProcessing = 5015,
	DriveInitializing = 5016,
	DriveWarmingUp = 5017,
	DriveTonerLow = 5018,
	DriveNoToner = 5019,
	DrivePagePunt = 5020,
	DriveUserIntervention = 5021,
	DriveOutOfMemory = 5022,
	DriveDoorOpen = 5023,
	DriveServerUnknown = 5024,
	DrivePowerSave = 5025,
	DriveServerOffline = 5026,
	DriveDriverUpdateNeeded = 5027,

	//打印与主机的网络环境预警(51)
	NetWorkDisconnectOfPrinter = 5101,
	NetWorkDisconnectOfCloud = 5102,
	NetWorkCloudRouterOffline = 5103,

	//MFP的硬件控制组件预警(52)，如复印控制器，电源控制器，arm硬件系统
	MFPControlHardwareDeviceInitFail = 5201,
	MFPControlHardwareCommunicateFail = 5202,
	MFPControlHardwarePowerOnFail = 5203,
	MFPControlHardwarePowerDownFail = 5204,
	MFPControlHardwareStartCopyFail = 5205,
	MFPControlHardwareStopCopyFail = 5206,
	MFPControlHardwareArmInitFail = 5207,

	//纸盒预警(53)
	TaryAlert = 5300,
	TaryPaper1Low = 5301,
	TaryPaper1Empty = 5302,
	TaryPaper2Low = 5303,
	TaryPaper2Empty = 5304,
	TaryPaper3Low = 5305,
	TaryPaper3Empty = 5306,
	TaryPaper4Low = 5307,
	TaryPaper4Empty = 5308,
	TaryPaperBypassLow = 5309,
	TaryPaperBypassEmpty = 5310,
	TaryPaperLow = 5311,
	TaryPaperEmpty = 5312,

	//粉盒预警(54)
	TonerAlert = 5400,
	TonerCyanLow = 5401,
	TonerCyanEmpty = 5402,
	TonerMagentaLow = 5403,
	TonerMagentaEmpty = 5404,
	TonerYellowLow = 5405,
	TonerYellowEmpty = 5406,
	TonerBlackLow = 5407,
	TonerBlackEmpty = 5408,
	TonerLow = 5409,
	TonerEmpty = 5410,

	//墨盒预警(55)
	InkAlert = 5500,
	InkCyanLow = 5501,
	InkCyanEmpty = 5502,
	InkMagentaLow = 5503,
	InkMagentaEmpty = 5504,
	InkYellowLow = 5505,
	InkYellowEmpty = 5506,
	InkBlackLow = 5507,
	InkBlackEmpty = 5508,
	InkLow = 5509,
	InkEmpty = 5510,

	//感光鼓预警(56)
	OPCAlert = 5600,
	OPCCyanLow = 5601,
	OPCCyanEmpty = 5602,
	OPCMagentaLow = 5603,
	OPCMagentaEmpty = 5604,
	OPCYellowLow = 5605,
	OPCYellowEmpty = 5606,
	OPCBlackLow = 5607,
	OPCBlackEmpty = 5608,
	OPCLow = 5609,
	OPCEmpty = 5610,

	//显影单元预警(57)
	DeveloperAlert = 5700,
	DeveloperCyanLow = 5701,
	DeveloperCyanEmpty = 5702,
	DeveloperMagentaLow = 5703,
	DeveloperMagentaEmpty = 5704,
	DeveloperYellowLow = 5705,
	DeveloperYellowEmpty = 5706,
	DeveloperBlackLow = 5707,
	DeveloperBlackEmpty = 5708,
	DeveloperLow = 5709,
	DeveloperEmpty = 5710,

	//打印机状态监视(58)
	PrinterMonitorCheckBegin = 5801,
	PrinterMonitorCheckProcess = 5802,
	PrinterMonitorCheckEnd = 5803,

	//打印机故障信息(59)
	PrinterFaultInfo = 5901
}
END_ENUM_STRUCT(PrinterAlertCode)

#endif	//_PRINTER_ALERT_DEF_F1C295E9_F82E_4E43_914E_D7F0E780854A_H_