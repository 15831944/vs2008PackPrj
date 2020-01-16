#ifndef _RFC1213_MIB_DEF_1C24B91E_987C_4CC8_AD4F_527E18F13AE7_H_
#define _RFC1213_MIB_DEF_1C24B91E_987C_4CC8_AD4F_527E18F13AE7_H_

#include "Snmp_Def.h"

/************************************************************************/
/* 本文档定义的结构类型参考下列文件：[iSecPrint\Doc\Snmp\Snmp通用MIB]
	1.RFC1213-MIB
	2.rfc3418.mi2.txt
	3.Net-SNMP.mht
*/
/************************************************************************/

/************************************************************************/
/* 
system       OBJECT IDENTIFIER ::= { mib-2 1 }

interfaces   OBJECT IDENTIFIER ::= { mib-2 2 }

at           OBJECT IDENTIFIER ::= { mib-2 3 }

ip           OBJECT IDENTIFIER ::= { mib-2 4 }

icmp         OBJECT IDENTIFIER ::= { mib-2 5 }

tcp          OBJECT IDENTIFIER ::= { mib-2 6 }

udp          OBJECT IDENTIFIER ::= { mib-2 7 }

egp          OBJECT IDENTIFIER ::= { mib-2 8 }

-- historical (some say hysterical)
-- cmot      OBJECT IDENTIFIER ::= { mib-2 9 }

transmission OBJECT IDENTIFIER ::= { mib-2 10 }

snmp         OBJECT IDENTIFIER ::= { mib-2 11 }
*/
/************************************************************************/

//sysORTable ::= { system 9 }
//sysOREntry ::= { sysORTable 1 }
#define SysOREntryOID TEXT("H1h9Vk0EfUVPQB0AR2JLWhxi")	//".1.3.6.1.2.1.1.9.1"
typedef struct tag_SysOREntry
{
	Integer32 sysORIndex;								//::= { sysOREntry 1 }
	OBJECT_IDENTIFIER sysORID[OBJECT_IDENTIFIER_SIZE];	//::= { sysOREntry 2 }
	OCTET_STRING sysORDescr[OCTET_STRING_SIZE_256];		//::= { sysOREntry 3 }
	TimeStampS sysORUpTime;								//::= { sysOREntry 4 }
}SysOREntry, *PSysOREntry;
typedef map<int, PSysOREntry> SysOREntryMap;
typedef list<PSysOREntry> SysOREntryList;

//system       OBJECT IDENTIFIER ::= { mib-2 1 }
#define SystemGroupOID TEXT("H1h9Vk0EfUVPQB0AR2I=")	//".1.3.6.1.2.1.1"=".iso.org.dod.internet.mgmt.mib-2.system"
typedef struct tag_SystemGroup
{
	OCTET_STRING sysDescr[OCTET_STRING_SIZE_256];			//::= { system 1 }	//0,只有一个数值
	OBJECT_IDENTIFIER sysObjectID[OBJECT_IDENTIFIER_SIZE];	//::= { system 2 }	//0,只有一个数值
	TimeTicks sysUpTime;									//::= { system 3 }	//0,只有一个数值
	OCTET_STRING sysContact[OCTET_STRING_SIZE_256];			//::= { system 4 }	//0,只有一个数值
	OCTET_STRING sysName[OCTET_STRING_SIZE_256];			//::= { system 5 }	//0,只有一个数值
	OCTET_STRING sysLocation[OCTET_STRING_SIZE_256];		//::= { system 6 }	//0,只有一个数值
	Integer32 sysServices;									//::= { system 7 }	//0,只有一个数值
	TimeStampS sysORLastChange;								//::= { system 8 }	//0,只有一个数值
	SysOREntryMap* sysORTable;								//::= { system 9 }
}SystemGroup, *PSystemGroup;
typedef map<int, PSystemGroup> SystemGroupMap;
typedef list<PSystemGroup> SystemGroupList;


//interfaces   OBJECT IDENTIFIER ::= { mib-2 2 }
BEGIN_ENUM_STRUCT(IfType)
{
	other = 1,         // -- none of the following
	regular1822 = 2,
	hdh1822 = 3,
	ddn_x25 = 4,
	rfc877_x25 = 5,
	ethernet_csmacd = 6,
	iso88023_csmacd = 7,
	iso88024_tokenBus = 8,
	iso88025_tokenRing = 9,
	iso88026_man = 10,
	starLan = 11,
	proteon_10Mbit = 12,
	proteon_80Mbit = 13,
	hyperchannel = 14,
	fddi = 15,
	lapb = 16,
	sdlc = 17,
	ds1 = 18,          // -- T-1
	e1 = 19,            //-- european equiv. of T-1
	basicISDN = 20,
	primaryISDN = 21,   //-- proprietary serial
	propPointToPointSerial = 22,
	ppp = 23,
	softwareLoopback = 24,
	eon = 25,            //-- CLNP over IP [11]
	ethernet_3Mbit = 26,
	nsip = 27,           //-- XNS over IP
	slip = 28,           //-- generic SLIP
	ultra = 29,          //-- ULTRA technologies
	ds3 = 30,            //-- T-3
	sip = 31,            //-- SMDS
	frame_relay = 32
		/*
	Value	Label/Meaning
	1	other
	2	regular1822
	3	hdh1822
	4	ddnX25
	5	rfc877x25
	6	ethernetCsmacd
	7	iso88023Csmacd
	8	iso88024TokenBus
	9	iso88025TokenRing
	10	iso88026Man
	11	starLan
	12	proteon10Mbit
	13	proteon80Mbit
	14	hyperchannel
	15	fddi
	16	lapb
	17	sdlc
	18	ds1
	19	e1
	20	basicISDN
	21	primaryISDN
	22	propPointToPointSerial
	23	ppp
	24	softwareLoopback
	25	eon
	26	ethernet3Mbit
	27	nsip
	28	slip
	29	ultra
	30	ds3
	31	sip
	32	frameRelay
	33	rs232
	34	para
	35	arcnet
	36	arcnetPlus
	37	atm
	38	miox25
	39	sonet
	40	x25ple
	41	iso88022llc
	42	localTalk
	43	smdsDxi
	44	frameRelayService
	45	v35
	46	hssi
	47	hippi
	48	modem
	49	aal5
	50	sonetPath
	51	sonetVT
	52	smdsIcip
	53	propVirtual
	54	propMultiplexor
	55	ieee80212
	56	fibreChannel
	57	hippiInterface
	58	frameRelayInterconnect
	59	aflane8023
	60	aflane8025
	61	cctEmul
	62	fastEther
	63	isdn
	64	v11
	65	v36
	66	g703at64k
	67	g703at2mb
	68	qllc
	69	fastEtherFX
	70	channel
	71	ieee80211
	72	ibm370parChan
	73	escon
	74	dlsw
	75	isdns
	76	isdnu
	77	lapd
	78	ipSwitch
	79	rsrb
	80	atmLogical
	81	ds0
	82	ds0Bundle
	83	bsc
	84	async
	85	cnr
	86	iso88025Dtr
	87	eplrs
	88	arap
	89	propCnls
	90	hostPad
	91	termPad
	92	frameRelayMPI
	93	x213
	94	adsl
	95	radsl
	96	sdsl
	97	vdsl
	98	iso88025CRFPInt
	99	myrinet
	100	voiceEM
	101	voiceFXO
	102	voiceFXS
	103	voiceEncap
	104	voiceOverIp
	105	atmDxi
	106	atmFuni
	107	atmIma
	108	pppMultilinkBundle
	109	ipOverCdlc
	110	ipOverClaw
	111	stackToStack
	112	virtualIpAddress
	113	mpc
	114	ipOverAtm
	115	iso88025Fiber
	116	tdlc
	117	gigabitEthernet
	118	hdlc
	119	lapf
	120	v37
	121	x25mlp
	122	x25huntGroup
	123	transpHdlc
	124	interleave
	125	fast
	126	ip
	127	docsCableMaclayer
	128	docsCableDownstream
	129	docsCableUpstream
	130	a12MppSwitch
	131	tunnel
	132	coffee
	133	ces
	134	atmSubInterface
	135	l2vlan
	136	l3ipvlan
	137	l3ipxvlan
	138	digitalPowerline
	139	mediaMailOverIp
	140	dtm
	141	dcn
	142	ipForward
	143	msdsl
	144	ieee1394
	145	if-gsn
	146	dvbRccMacLayer
	147	dvbRccDownstream
	148	dvbRccUpstream
	149	atmVirtual
	150	mplsTunnel
	151	srp
	152	voiceOverAtm
	153	voiceOverFrameRelay
	154	idsl
	155	compositeLink
	156	ss7SigLink
	157	propWirelessP2P
	158	frForward
	159	rfc1483
	160	usb
	161	ieee8023adLag
	162	bgppolicyaccounting
	163	frf16MfrBundle
	164	h323Gatekeeper
	165	h323Proxy
	166	mpls
	167	mfSigLink
	168	hdsl2
	169	shdsl
	170	ds1FDL
	171	pos
	172	dvbAsiIn
	173	dvbAsiOut
	174	plc
	175	nfas
	176	tr008
	177	gr303RDT
	178	gr303IDT
	179	isup
	180	propDocsWirelessMaclayer
	181	propDocsWirelessDownstream
	182	propDocsWirelessUpstream
	183	hiperlan2
	184	propBWAp2Mp
	185	sonetOverheadChannel
	186	digitalWrapperOverheadChannel
	187	aal2
	188	radioMAC
	189	atmRadio
	190	imt
	191	mvl
	192	reachDSL
	193	frDlciEndPt
	194	atmVciEndPt
	195	opticalChannel
	196	opticalTransport
	197	propAtm
	198	voiceOverCable
	199	infiniband
	200	teLink
	201	q2931
	202	virtualTg
	203	sipTg
	204	sipSig
	205	docsCableUpstreamChannel
	206	econet
	207	pon155
	208	pon622
	209	bridge
	210	linegroup
	211	voiceEMFGD
	212	voiceFGDEANA
	213	voiceDID
	214	mpegTransport
	215	sixToFour
	216	gtp
	217	pdnEtherLoop1
	218	pdnEtherLoop2
	219	opticalChannelGroup
	220	homepna
	221	gfp
	222	ciscoISLvlan
	223	actelisMetaLOOP
	224	fcipLink
	225	rpr
	226	qam
	227	lmp
	228	cblVectaStar
	229	docsCableMCmtsDownstream
	230	adsl2
	231	macSecControlledIF
	232	macSecUncontrolledIF
	233	aviciOpticalEther
	234	atmbond
	235	voiceFGDOS
	236	mocaVersion1
	237	ieee80216WMAN
	238	adsl2plus
	239	dvbRcsMacLayer
	240	dvbTdm
	241	dvbRcsTdma
	242	x86Laps
	243	wwanPP
	244	wwanPP2
	245	voiceEBS
	246	ifPwType
	247	ilan
	248	pip
	249	aluELP
	250	gpon
	251	vdsl2
	252	capwapDot11Profile
	253	capwapDot11Bss
	254	capwapWtpVirtualRadio
	255	bits
	256	docsCableUpstreamRfPort
	257	cableDownstreamRfPort
	258	vmwareVirtualNic
	259	ieee802154
	260	otnOdu
	261	otnOtu
	262	ifVfiType
	263	g9981
	264	g9982
	265	g9983
	266	aluEpon
	267	aluEponOnu
	268	aluEponPhysicalUni
	269	aluEponLogicalLink
	270	aluGponOnu
	271	aluGponPhysicalUni
	272	vmwareNicTeam
	*/
}
END_ENUM_STRUCT(IfType)

BEGIN_ENUM_STRUCT(IfAdminStatus)
{
	up = 1,       //-- ready to pass packets
	down = 2,
	testing = 3   //-- in some test mode
}
END_ENUM_STRUCT(IfAdminStatus)

BEGIN_ENUM_STRUCT(IfOperStatus)
{
	up = 1,       //-- ready to pass packets
	down = 2,
	testing = 3,   //-- in some test mode
	unknown = 4,
	dormant = 5,
	notPresent = 6,
	lowerLayerDown = 7
}
END_ENUM_STRUCT(IfOperStatus)


//ifTable ::= { interfaces 2 }
//ifEntry ::= { ifTable 1 }
//"interfaces.ifTable.ifEntry"=".iso.org.dod.internet.mgmt.mib-2.interfaces.ifTable.ifEntry"
#define IfEntryOID						TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRw=")							//"interfaces.ifTable.ifEntry"
#define IfIndexOID						TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU9DxZWSQ==")				//"interfaces.ifTable.ifEntry.ifIndex"
#define IfDescrOID						TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzUwBAFQQw==")				//"interfaces.ifTable.ifEntry.ifDescr"
#define IfTypeOID						TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzUgGAJW")					//"interfaces.ifTable.ifEntry.ifType"
#define IfMtuOID						TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU5FQc=")					//"interfaces.ifTable.ifEntry.ifMtu"
#define IfSpeedOID						TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzUnERdWVQ==")				//"interfaces.ifTable.ifEntry.ifSpeed"
#define IfPhysAddressOID				TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzUkCQtAcA03FwZBIA==")		//"interfaces.ifTable.ifEntry.ifPhysAddress"
#define IfAdminStatusOID				TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU1BR9aXzonBBdHIA==")		//"interfaces.ifTable.ifEntry.ifAdminStatus"
#define IfOperStatusOID					TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU7ERdBYh0yERZB")			//"interfaces.ifTable.ifEntry.ifOperStatus"
#define IfLastChangeOID					TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU4AAFHcgEyCwRX")			//"interfaces.ifTable.ifEntry.ifLastChange"
#define IfInOctetsOID					TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU9Dz1QRQwnFg==")			//"interfaces.ifTable.ifEntry.ifInOctets"
#define IfInUcastPktsOID				TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU9DydQUBonNQhGIA==")		//"interfaces.ifTable.ifEntry.ifInUcastPkts"
#define IfInNUcastPktsOID				TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU9DzxmUgggETNZJwc=")		//"interfaces.ifTable.ifEntry.ifInNUcastPkts"
#define IfInDiscardsOID					TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU9DzZaQgoyFwdB")			//"interfaces.ifTable.ifEntry.ifInDiscards"
#define IfInErrorsOID					TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU9DzdBQwYhFg==")			//"interfaces.ifTable.ifEntry.ifInErrors"
#define IfInUnknownProtosOID			TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU9DyddWgc8Eg1iIRsVHUA=")	//"interfaces.ifTable.ifEntry.ifInUnknownProtos"
#define IfOutOctetsOID					TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU7FAZ8Uh02ERA=")			//"interfaces.ifTable.ifEntry.ifOutOctets"
#define IfOutUcastPktsOID				TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU7FAZmUgggETNZJwc=")		//"interfaces.ifTable.ifEntry.ifOutUcastPkts"
#define IfOutNUcastPktsOID				TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU7FAZ9ZAoyFhdiOAAS")		//"interfaces.ifTable.ifEntry.ifOutNUcastPkts"
#define IfOutDiscardsOID				TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU7FAZ3WBowBBFWIA==")		//"interfaces.ifTable.ifEntry.ifOutDiscards"
#define IfOutErrorsOID					TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU7FAZ2Qxs8FxA=")			//"interfaces.ifTable.ifEntry.ifOutErrors"
#define IfOutQLenOID					TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzU7FAZifQw9")				//"interfaces.ifTable.ifEntry.ifOutQLen"
#define IfSpecificOID					TEXT("WAcnABFUMhcEAR1YDwcEAV42WggUdl8dIRxNWzUnERdQWA86Bg==")			//"interfaces.ifTable.ifEntry.ifSpecific"
typedef struct tag_IfEntry
{
	Integer32 ifIndex;									//::= { ifEntry 1 }
	DisplayString ifDescr[OCTET_STRING_SIZE_256];		//::= { ifEntry 2 }
	ENUM_STRUCT_TYPE(IfType) ifType;					//::= { ifEntry 3 }
	Integer32 ifMtu;									//::= { ifEntry 4 }
	Gauge32 ifSpeed;									//::= { ifEntry 5 }
	PhysAddress ifPhysAddress[PhysAddress_SIZE];		//::= { ifEntry 6 }
	ENUM_STRUCT_TYPE(IfAdminStatus) ifAdminStatus;		//::= { ifEntry 7 }
	ENUM_STRUCT_TYPE(IfOperStatus) ifOperStatus;		//::= { ifEntry 8 }
	TimeTicks ifLastChange;								//::= { ifEntry 9 }
	Counter32 ifInOctets;								//::= { ifEntry 10 }
	Counter32 ifInUcastPkts;							//::= { ifEntry 11 }
	Counter32 ifInNUcastPkts;							//::= { ifEntry 12 }
	Counter32 ifInDiscards;								//::= { ifEntry 13 }
	Counter32 ifInErrors;								//::= { ifEntry 14 }
	Counter32 ifInUnknownProtos;						//::= { ifEntry 15 }
	Counter32 ifOutOctets;								//::= { ifEntry 16 }
	Counter32 ifOutUcastPkts;							//::= { ifEntry 17 }
	Counter32 ifOutNUcastPkts;							//::= { ifEntry 18 }
	Counter32 ifOutDiscards;							//::= { ifEntry 19 }
	Counter32 ifOutErrors;								//::= { ifEntry 20 }
	Gauge32 ifOutQLen;									//::= { ifEntry 21 }
	OBJECT_IDENTIFIER ifSpecific[OBJECT_IDENTIFIER_SIZE];			//::= { ifEntry 22 }
}IfEntry, *PIfEntry;
typedef map<int, PIfEntry> IfEntryMap;
typedef list<PIfEntry> IfEntryList;

#define InterfacesGroupOID TEXT("H1h9Vk0EfUVPQB0AR2E=")	//".1.3.6.1.2.1.2"=".iso.org.dod.internet.mgmt.mib-2.interfaces"
typedef struct tag_InterfacesGroup
{
	Integer32 ifNumber;								//::= { interfaces 1 }	//0,只有一个数值
	IfEntryMap* ifTable;						//::= { interfaces 2 }
}InterfacesGroup, *PInterfacesGroup;
typedef map<int, PInterfacesGroup> InterfacesGroupMap;
typedef list<PInterfacesGroup> InterfacesGroupList;


#endif	//_RFC1213_MIB_DEF_1C24B91E_987C_4CC8_AD4F_527E18F13AE7_H_
