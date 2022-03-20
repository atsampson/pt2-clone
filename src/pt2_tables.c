#include <stdint.h>
#include <stdbool.h>
#include "pt2_mouse.h"

const char *ftuneStrTab[16] =
{
	" 0", "+1", "+2", "+3",
	"+4", "+5", "+6", "+7",
	"-8", "-7", "-6", "-5",
	"-4", "-3", "-2", "-1"
};

const int8_t vuMeterHeights[65] =
{
	 0,  0,  1,  2,  2,  3,  4,  5,
	 5,  6,  7,  8,  8,  9, 10, 11,
	11, 12, 13, 14, 14, 15, 16, 17,
	17, 18, 19, 20, 20, 21, 22, 23,
	23, 24, 25, 26, 26, 27, 28, 29,
	29, 30, 31, 32, 32, 33, 34, 35,
	35, 36, 37, 38, 38, 39, 40, 41,
	41, 42, 43, 44, 44, 45, 46, 47,
	47
};

const char hexTable[16] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

const uint32_t cursorColors[6][3] =
{
	{ 0x444444, 0x777777, 0xAAAAAA }, // gray
	{ 0x444400, 0x777700, 0xAAAA00 }, // yellow
	{ 0x000066, 0x004499, 0x0055BB }, // blue
	{ 0x440044, 0x770077, 0xAA00AA }, // purple
	{ 0x004400, 0x007700, 0x00AA00 }, // green
	{ 0x770000, 0x990000, 0xCC0000 }  // red
};

int8_t pNoteTable[32] = // for drumpad
{
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24
};

const char *noteNames1[2+36] =
{
	"---","???",
	"C-1", "C#1", "D-1", "D#1", "E-1", "F-1", "F#1" ,"G-1", "G#1", "A-1", "A#1", "B-1",
	"C-2", "C#2", "D-2", "D#2", "E-2", "F-2", "F#2" ,"G-2", "G#2", "A-2", "A#2", "B-2",
	"C-3", "C#3", "D-3", "D#3", "E-3", "F-3", "F#3" ,"G-3", "G#3", "A-3", "A#3", "B-3"
};

const char *noteNames2[2+36] =
{
	"---","???",
	"C-1", "D\x01""1", "D-1", "E\x01""1", "E-1", "F-1", "G\x01""1", "G-1", "A\x01""1", "A-1", "B\x01""1", "B-1",
	"C-2", "D\x01""2", "D-2", "E\x01""2", "E-2", "F-2", "G\x01""2", "G-2", "A\x01""2", "A-2", "B\x01""2", "B-2",
	"C-3", "D\x01""3", "D-3", "E\x01""3", "E-3", "F-3", "G\x01""3", "G-3", "A\x01""3", "A-3", "B\x01""3", "B-3"
};

const char *noteNames3[2+36] = // for PATTDOTS mode
{
	"\x02\x02\x02","???",
	"C-1", "C#1", "D-1", "D#1", "E-1", "F-1", "F#1" ,"G-1", "G#1", "A-1", "A#1", "B-1",
	"C-2", "C#2", "D-2", "D#2", "E-2", "F-2", "F#2" ,"G-2", "G#2", "A-2", "A#2", "B-2",
	"C-3", "C#3", "D-3", "D#3", "E-3", "F-3", "F#3" ,"G-3", "G#3", "A-3", "A#3", "B-3"
};

const char *noteNames4[2+36] = // for PATTDOTS mode
{
	"\x02\x02\x02","???",
	"C-1", "D\x01""1", "D-1", "E\x01""1", "E-1", "F-1", "G\x01""1", "G-1", "A\x01""1", "A-1", "B\x01""1", "B-1",
	"C-2", "D\x01""2", "D-2", "E\x01""2", "E-2", "F-2", "G\x01""2", "G-2", "A\x01""2", "A-2", "B\x01""2", "B-2",
	"C-3", "D\x01""3", "D-3", "E\x013""", "E-3", "F-3", "G\x01""3", "G-3", "A\x01""3", "A-3", "B\x01""3", "B-3"
};

const uint8_t vibratoTable[32] =
{
	0x00, 0x18, 0x31, 0x4A, 0x61, 0x78, 0x8D, 0xA1,
	0xB4, 0xC5, 0xD4, 0xE0, 0xEB, 0xF4, 0xFA, 0xFD,
	0xFF, 0xFD, 0xFA, 0xF4, 0xEB, 0xE0, 0xD4, 0xC5,
	0xB4, 0xA1, 0x8D, 0x78, 0x61, 0x4A, 0x31, 0x18
};

const int16_t periodTable[(37*16)+15] = // contains 16 finetuned period sections
{
	// finetune 0 (no finetuning)
	856,808,762,720,678,640,604,570,538,508,480,453,
	428,404,381,360,339,320,302,285,269,254,240,226,
	214,202,190,180,170,160,151,143,135,127,120,113,0,
	
	// finetune +1
	850,802,757,715,674,637,601,567,535,505,477,450,
	425,401,379,357,337,318,300,284,268,253,239,225,
	213,201,189,179,169,159,150,142,134,126,119,113,0,

	// finetune +2
	844,796,752,709,670,632,597,563,532,502,474,447,
	422,398,376,355,335,316,298,282,266,251,237,224,
	211,199,188,177,167,158,149,141,133,125,118,112,0,

	// finetune +3
	838,791,746,704,665,628,592,559,528,498,470,444,
	419,395,373,352,332,314,296,280,264,249,235,222,
	209,198,187,176,166,157,148,140,132,125,118,111,0,

	// finetune +4
	832,785,741,699,660,623,588,555,524,495,467,441,
	416,392,370,350,330,312,294,278,262,247,233,220,
	208,196,185,175,165,156,147,139,131,124,117,110,0,

	// finetune +5
	826,779,736,694,655,619,584,551,520,491,463,437,
	413,390,368,347,328,309,292,276,260,245,232,219,
	206,195,184,174,164,155,146,138,130,123,116,109,0,

	// finetune +6
	820,774,730,689,651,614,580,547,516,487,460,434,
	410,387,365,345,325,307,290,274,258,244,230,217,
	205,193,183,172,163,154,145,137,129,122,115,109,0,

	// finetune +7
	814,768,725,684,646,610,575,543,513,484,457,431,
	407,384,363,342,323,305,288,272,256,242,228,216,
	204,192,181,171,161,152,144,136,128,121,114,108,0,

	// finetune -8
	907,856,808,762,720,678,640,604,570,538,508,480,
	453,428,404,381,360,339,320,302,285,269,254,240,
	226,214,202,190,180,170,160,151,143,135,127,120,0,

	// finetune -7
	900,850,802,757,715,675,636,601,567,535,505,477,
	450,425,401,379,357,337,318,300,284,268,253,238,
	225,212,200,189,179,169,159,150,142,134,126,119,0,

	// finetune -6
	894,844,796,752,709,670,632,597,563,532,502,474,
	447,422,398,376,355,335,316,298,282,266,251,237,
	223,211,199,188,177,167,158,149,141,133,125,118,0,

	// finetune -5
	887,838,791,746,704,665,628,592,559,528,498,470,
	444,419,395,373,352,332,314,296,280,264,249,235,
	222,209,198,187,176,166,157,148,140,132,125,118,0,

	// finetune -4
	881,832,785,741,699,660,623,588,555,524,494,467,
	441,416,392,370,350,330,312,294,278,262,247,233,
	220,208,196,185,175,165,156,147,139,131,123,117,0,

	// finetune -3
	875,826,779,736,694,655,619,584,551,520,491,463,
	437,413,390,368,347,328,309,292,276,260,245,232,
	219,206,195,184,174,164,155,146,138,130,123,116,0,

	// finetune -2
	868,820,774,730,689,651,614,580,547,516,487,460,
	434,410,387,365,345,325,307,290,274,258,244,230,
	217,205,193,183,172,163,154,145,137,129,122,115,0,

	// finetune -1
	862,814,768,725,684,646,610,575,543,513,484,457,
	431,407,384,363,342,323,305,288,272,256,242,228,
	216,203,192,181,171,161,152,144,136,128,121,114,0,

	/* Arpeggio on -1 finetuned samples can do an out-of-bounds read from
	** this table. Here's the correct overflow values from the
	** "CursorPosTable" and "UnshiftedKeymap" table in the PTcode, which are
	** located right after the period table. These tables and their order didn't
	** seem to change in the different PT1.x/PT2.x versions (I checked the
	** source codes).
	*/
	774,1800,2314,3087,4113,4627,5400,6426,6940,7713,
	8739,9253,24625,12851,13365
};

/*
** const double dBpmMs1024 = 1024.0 / (bpm / 2.5); // milliseconds (scaled from 1000 to 1024)
** x = (uint64_t)floor((UINT32_MAX + 1.0) * dBpmMs1024);
*/
const uint64_t musicTimeTab64[256-32] =
{
	0x0000000000,0x4D9364D936,0x4B4B4B4B4B,0x4924924924,0x471C71C71C,0x45306EB3E4,
	0x435E50D794,0x41A41A41A4,0x4000000000,0x3E7063E706,0x3CF3CF3CF3,0x3B88EE23B8,
	0x3A2E8BA2E8,0x38E38E38E3,0x37A6F4DE9B,0x3677D46CEF,0x3555555555,0x343EB1A1F5,
	0x3333333333,0x3232323232,0x313B13B13B,0x304D4873EC,0x2F684BDA12,0x2E8BA2E8BA,
	0x2DB6DB6DB6,0x2CE98B3A62,0x2C234F72C2,0x2B63CBEEA4,0x2AAAAAAAAA,0x29F79B4758,
	0x294A5294A5,0x28A28A28A2,0x2800000000,0x2762762762,0x26C9B26C9B,0x26357E16EC,
	0x25A5A5A5A5,0x2519F89467,0x2492492492,0x240E6C2B44,0x238E38E38E,0x231188C462,
	0x22983759F2,0x2222222222,0x21AF286BCA,0x213F2B3884,0x20D20D20D2,0x2067B23A54,
	0x2000000000,0x1F9ADD3C0C,0x1F3831F383,0x1ED7E75346,0x1E79E79E79,0x1E1E1E1E1E,
	0x1DC47711DC,0x1D6CDFA1D6,0x1D1745D174,0x1CC398730E,0x1C71C71C71,0x1C21C21C21,
	0x1BD37A6F4D,0x1B86E1B86E,0x1B3BEA3677,0x1AF286BCA1,0x1AAAAAAAAA,0x1A6449E59B,
	0x1A1F58D0FA,0x19DBCC4867,0x1999999999,0x1958B67EBB,0x1919191919,0x18DAB7EC1D,
	0x189D89D89D,0x1861861861,0x1826A439F6,0x17ECDC1CB5,0x17B425ED09,0x177C7A20E1,
	0x1745D1745D,0x171024E6A1,0x16DB6DB6DB,0x16A7A5616A,0x1674C59D31,0x1642C8590B,
	0x1611A7B961,0x15E15E15E1,0x15B1E5F752,0x15833A1583,0x1555555555,0x152832C6E0,
	0x14FBCDA3AC,0x14D0214D02,0x14A5294A52,0x147AE147AE,0x1451451451,0x142850A142,
	0x1400000000,0x13D84F613D,0x13B13B13B1,0x138ABF82EE,0x1364D9364D,0x133F84CFE1,
	0x131ABF0B76,0x12F684BDA1,0x12D2D2D2D2,0x12AFA64E7B,0x128CFC4A33,0x126AD1F4F3,
	0x1249249249,0x1227F179A5,0x12073615A2,0x11E6EFE35B,0x11C71C71C7,0x11A7B9611A,
	0x1188C46231,0x116A3B35FC,0x114C1BACF9,0x112E63A6A8,0x1111111111,0x10F421E843,
	0x10D79435E5,0x10BB6610BB,0x109F959C42,0x1084210842,0x1069069069,0x104E447BEC,
	0x1033D91D2A,0x1019C2D14E,0x1000000000,0x0FE68F1B07,0x0FCD6E9E06,0x0FB49D0E22,
	0x0F9C18F9C1,0x0F83E0F83E,0x0F6BF3A9A3,0x0F544FB66B,0x0F3CF3CF3C,0x0F25DEACAF,
	0x0F0F0F0F0F,0x0EF883BE20,0x0EE23B88EE,0x0ECC35458C,0x0EB66FD0EB,0x0EA0EA0EA0,
	0x0E8BA2E8BA,0x0E76994F8C,0x0E61CC3987,0x0E4D3AA30A,0x0E38E38E38,0x0E24C602D4,
	0x0E10E10E10,0x0DFD33C272,0x0DE9BD37A6,0x0DD67C8A60,0x0DC370DC37,0x0DB0995382,
	0x0D9DF51B3B,0x0D8B8362E0,0x0D79435E50,0x0D673445B2,0x0D55555555,0x0D43A5CD98,
	0x0D3224F2CD,0x0D20D20D20,0x0D0FAC687D,0x0CFEB35477,0x0CEDE62433,0x0CDD442E4F,
	0x0CCCCCCCCC,0x0CBC7F5CF9,0x0CAC5B3F5D,0x0C9C5FD7A5,0x0C8C8C8C8C,0x0C7CE0C7CE,
	0x0C6D5BF60E,0x0C5DFD86CD,0x0C4EC4EC4E,0x0C3FB19B8F,0x0C30C30C30,0x0C21F8B86A,
	0x0C13521CFB,0x0C04CEB916,0x0BF66E0E5A,0x0BE82FA0BE,0x0BDA12F684,0x0BCC17982F,
	0x0BBE3D1070,0x0BB082EC20,0x0BA2E8BA2E,0x0B956E0B95,0x0B88127350,0x0B7AD58650,
	0x0B6DB6DB6D,0x0B60B60B60,0x0B53D2B0B5,0x0B470C67C0,0x0B3A62CE98,0x0B2DD58507,
	0x0B21642C85,0x0B150E682C,0x0B08D3DCB0,0x0AFCB43057,0x0AF0AF0AF0,0x0AE4C415C9,
	0x0AD8F2FBA9,0x0ACD3B68C6,0x0AC19D0AC1,0x0AB617909A,0x0AAAAAAAAA,0x0A9F560A9F,
	0x0A94196370,0x0A88F46959,0x0A7DE6D1D6,0x0A72F05397,0x0A6810A681,0x0A5D4783A0,
	0x0A5294A529,0x0A47F7C66C,0x0A3D70A3D7,0x0A32FEFAE6,0x0A28A28A28,0x0A1E5B1133,
	0x0A142850A1,0x0A0A0A0A0A
};

// button tables taken from the ptplay project + modified

const guiButton_t bAsk[] =
{
	{171, 71,196, 81, PTB_SUREY},
	{234, 71,252, 81, PTB_SUREN}
};

const guiButton_t bPat2SmpAsk[] =
{
	{168, 71,185, 81, PTB_PAT2SMP_HI},
	{192, 71,210, 81, PTB_PAT2SMP_LO},
	{217, 71,256, 81, PTB_PAT2SMP_ABORT}
};

const guiButton_t bClear[] =
{
	{166, 57,198, 67, PTB_CLEARSONG},
	{204, 57,257, 67, PTB_CLEARSAMPLES},

	{166, 73,198, 83, PTB_CLEARALL},
	{204, 73,257, 83, PTB_CLEARCANCEL}
};

const guiButton_t bTopScreen[] =
{
	{  0,  0, 39, 10, PTB_POSED},
	{ 40,  0, 50, 10, PTB_POSINS},
	{ 51,  0, 61, 10, PTB_POSDEL},
	{ 62,  0, 97, 10, PTB_POSS},
	{ 98,  0,108, 10, PTB_POSU},
	{109,  0,119, 10, PTB_POSD},
	{120,  0,181, 10, PTB_PLAY},
	{182,  0,243, 10, PTB_STOP},
	{244,  0,305, 10, PTB_MOD2WAV},
	{306,  0,319, 10, PTB_CHAN1},

	{ 62, 11, 97, 21, PTB_PATTERNS},
	{ 98, 11,108, 21, PTB_PATTERNU},
	{109, 11,119, 21, PTB_PATTERND},
	{120, 11,181, 21, PTB_PATTERN},
	{182, 11,243, 21, PTB_CLEAR},
	{244, 11,305, 21, PTB_PAT2SMP},
	{306, 11,319, 21, PTB_CHAN2},

	{ 62, 22, 97, 32, PTB_LENGTHS},
	{ 98, 22,108, 32, PTB_LENGTHU},
	{109, 22,119, 32, PTB_LENGTHD},
	{120, 22,181, 32, PTB_EDIT},
	{182, 22,243, 32, PTB_EDITOP},
	{244, 22,305, 32, PTB_POSED},
	{306, 22,319, 32, PTB_CHAN3},

	{ 98, 33,108, 43, PTB_FTUNEU},
	{109, 33,119, 43, PTB_FTUNED},
	{120, 33,181, 43, PTB_RECORD},
	{182, 33,243, 43, PTB_DISKOP},
	{244, 33,305, 43, PTB_SAMPLER},
	{306, 33,319, 43, PTB_CHAN4},

	{ 62, 44, 97, 54, PTB_SAMPLES},
	{ 98, 44,108, 54, PTB_SAMPLEU},
	{109, 44,119, 54, PTB_SAMPLED},
	{306, 44,319, 54, PTB_ABOUT}, // 'about' has priority over PTB_VISUALS
	{120, 44,319, 98, PTB_VISUALS},

	{ 62, 55, 97, 65, PTB_SVOLUMES},
	{ 98, 55,108, 65, PTB_SVOLUMEU},
	{109, 55,119, 65, PTB_SVOLUMED},

	{ 54, 66, 97, 76, PTB_SLENGTHS},
	{ 98, 66,108, 76, PTB_SLENGTHU},
	{109, 66,119, 76, PTB_SLENGTHD},

	{ 54, 77, 97, 87, PTB_SREPEATS},
	{ 98, 77,108, 87, PTB_SREPEATU},
	{109, 77,119, 87, PTB_SREPEATD},

	{ 54, 88, 97, 98, PTB_SREPLENS},
	{ 98, 88,108, 98, PTB_SREPLENU},
	{109, 88,119, 98, PTB_SREPLEND}
};

const guiButton_t bMidScreen[] =
{
	{  0, 99,319,109, PTB_SONGNAME},

	{  0,110,286,120, PTB_SAMPLENAME},
	{287,110,319,120, PTB_LOADSAMPLE}
};

const guiButton_t bBotScreen[] =
{
	{  0,121, 25,137, PTB_PATTBOX},
	{ 26,121, 43,137, PTB_TEMPOU},
	{ 43,121, 59,137, PTB_TEMPOD},

	{  0,138,319,254, PTB_PATTDATA}
};

const guiButton_t bDiskOp[] =
{
	{ 80,  0,145, 10, PTB_DO_PACKMOD},
	{146,  0,155, 10, PTB_DO_MODARROW},
	{156,  0,237, 10, PTB_DO_LOADMODULE},
	{238,  0,319, 10, PTB_DO_SAVEMODULE},

	{ 80, 11,145, 21, PTB_DO_SAMPLEFORMAT},
	{146, 11,155, 21, PTB_DO_SAMPLEARROW},
	{156, 11,237, 21, PTB_DO_LOADSAMPLE},
	{238, 11,319, 21, PTB_DO_SAVESAMPLE},

	{  0, 22,237, 32, PTB_DO_DATAPATH},
	{238, 22,272, 32, PTB_DO_PARENT},
	{273, 22,307, 32, PTB_DO_REFRESH},
	{308, 22,319, 30, PTB_DO_SCROLLTOP},

	{308, 31,319, 39, PTB_DO_SCROLLUP},
	{  2, 34,304, 93, PTB_DO_FILEAREA},
	{308, 40,319, 80, PTB_DO_EXIT},
	{308, 81,319, 89, PTB_DO_SCROLLDOWN},
	{308, 90,319, 99, PTB_DO_SCROLLBOT},
};

const guiButton_t bPosEd[] =
{
	{ 120,  0,171, 10, PTB_POSINS},
	{ 172,  0,267, 21, PTB_STOP},
	{ 268,  0,319, 10, PTB_PLAY},

	{ 120, 11,171, 21, PTB_POSDEL},
	{ 178, 22,307, 98, PTB_DUMMY},
	{ 268, 11,319, 21, PTB_PATTERN},

	{ 120, 22,177, 98, PTB_PE_PATT},
	{ 308, 22,319, 32, PTB_PE_SCROLLTOP},

	{ 308, 33,319, 43, PTB_PE_SCROLLUP},
	{ 308, 44,319, 76, PTB_PE_EXIT},
	{ 308, 77,319, 87, PTB_PE_SCROLLDOWN},
	{ 308, 88,319, 98, PTB_PE_SCROLLBOT}
};

const guiButton_t bEditOp1[] =
{
	{120, 44,319, 54, PTB_EO_TITLEBAR},

	{120, 55,212, 65, PTB_EO_TRACK_NOTE_UP},
	{213, 55,305, 65, PTB_EO_PATT_NOTE_UP},
	{306, 55,319, 65, PTB_DUMMY},

	{120, 66,212, 76, PTB_EO_TRACK_NOTE_DOWN},
	{213, 66,305, 76, PTB_EO_PATT_NOTE_DOWN},
	{306, 66,319, 76, PTB_EO_2},

	{120, 77,212, 87, PTB_EO_TRACK_OCTA_UP},
	{213, 77,305, 87, PTB_EO_PATT_OCTA_UP},
	{306, 77,319, 87, PTB_EO_3},

	{120, 88,212, 98, PTB_EO_TRACK_OCTA_DOWN},
	{213, 88,305, 98, PTB_EO_PATT_OCTA_DOWN},
	{306, 88,319, 98, PTB_EO_EXIT}
};

const guiButton_t bEditOp2[] =
{
	{120, 44,319, 54, PTB_EO_TITLEBAR},

	{120, 55,212, 65, PTB_EO_RECORD},
	{213, 55,259, 65, PTB_EO_DELETE},
	{260, 55,305, 65, PTB_EO_KILL},
	{306, 55,319, 65, PTB_EO_1},

	{120, 66,212, 76, PTB_EO_QUANTIZE},
	{213, 66,259, 76, PTB_EO_EXCHGE},
	{260, 66,305, 76, PTB_EO_COPY},
	{306, 66,319, 76, PTB_DUMMY},

	{120, 77,188, 87, PTB_EO_METRO_1},
	{189, 77,212, 87, PTB_EO_METRO_2},
	{213, 77,259, 87, PTB_EO_FROM},
	{260, 77,283, 87, PTB_EO_FROM_NUM},
	{284, 77,294, 87, PTB_EO_FROM_UP},
	{295, 77,305, 87, PTB_EO_FROM_DOWN},
	{306, 77,319, 87, PTB_EO_3},

	{120, 88,212, 98, PTB_EO_KEYS},
	{213, 88,259, 98, PTB_EO_TO},
	{260, 88,283, 98, PTB_EO_TO_NUM},
	{284, 88,294, 98, PTB_EO_TO_UP},
	{295, 88,305, 98, PTB_EO_TO_DOWN},
	{306, 88,319, 98, PTB_EO_EXIT},
};

const guiButton_t bEditOp3[] =
{
	{120, 44,319, 54, PTB_EO_TITLEBAR},

	{120, 55,165, 65, PTB_EO_MIX},
	{166, 55,212, 65, PTB_EO_ECHO},
	{213, 55,237, 65, PTB_DUMMY},
	{238, 55,283, 65, PTB_EO_POS_NUM},
	{284, 55,294, 65, PTB_EO_POS_UP},
	{295, 55,305, 65, PTB_EO_POS_DOWN},
	{306, 55,319, 65, PTB_EO_1},

	{120, 66,165, 76, PTB_EO_BOOST},
	{166, 66,212, 76, PTB_EO_FILTER},
	{213, 66,243, 76, PTB_EO_MOD},
	{244, 66,283, 76, PTB_EO_MOD_NUM},
	{284, 66,294, 76, PTB_EO_MOD_UP},
	{295, 66,305, 76, PTB_EO_MOD_DOWN},
	{306, 66,319, 76, PTB_EO_2},

	{120, 77,165, 87, PTB_EO_X_FADE},
	{166, 77,212, 87, PTB_EO_BACKWD},
	{213, 77,230, 87, PTB_EO_CB},
	{231, 77,269, 87, PTB_EO_CHORD},
	{270, 77,287, 87, PTB_EO_FU},
	{288, 77,305, 87, PTB_EO_FD},
	{306, 77,319, 87, PTB_DUMMY},

	{120, 88,165, 98, PTB_EO_UPSAMP},
	{166, 88,212, 98, PTB_EO_DNSAMP},
	{213, 88,243, 98, PTB_EO_VOL},
	{244, 88,283, 98, PTB_EO_VOL_NUM},
	{284, 88,294, 98, PTB_EO_VOL_UP},
	{295, 88,305, 98, PTB_EO_VOL_DOWN},
	{306, 88,319, 98, PTB_EO_EXIT}
};

const guiButton_t bEditOp4[] =
{
	{120, 44,319, 54, PTB_EO_TITLEBAR},

	{120, 55,165, 65, PTB_EO_DOCHORD},
	{166, 55,204, 65, PTB_EO_MAJOR},
	{205, 55,251, 65, PTB_EO_MAJOR7},
	{251, 55,283, 65, PTB_EO_NOTE1},
	{284, 55,294, 65, PTB_EO_NOTE1_UP},
	{295, 55,305, 65, PTB_EO_NOTE1_DOWN},
	{306, 55,319, 65, PTB_EO_1},

	{120, 66,165, 76, PTB_EO_RESET},
	{166, 66,204, 76, PTB_EO_MINOR},
	{205, 66,251, 76, PTB_EO_MINOR7},
	{251, 66,283, 76, PTB_EO_NOTE2},
	{284, 66,294, 76, PTB_EO_NOTE2_UP},
	{295, 66,305, 76, PTB_EO_NOTE2_DOWN},
	{306, 66,319, 76, PTB_EO_2},

	{120, 77,165, 87, PTB_EO_UNDO},
	{166, 77,204, 87, PTB_EO_SUS4},
	{205, 77,251, 87, PTB_EO_MAJOR6},
	{251, 77,283, 87, PTB_EO_NOTE3},
	{284, 77,294, 87, PTB_EO_NOTE3_UP},
	{295, 77,305, 87, PTB_EO_NOTE3_DOWN},
	{306, 77,319, 87, PTB_EO_3},

	{120, 88,165, 98, PTB_EO_LENGTH},
	{166, 88,204, 98, PTB_DUMMY},
	{205, 88,251, 98, PTB_EO_MINOR6},
	{251, 88,283, 98, PTB_EO_NOTE4},
	{284, 88,294, 98, PTB_EO_NOTE4_UP},
	{295, 88,305, 98, PTB_EO_NOTE4_DOWN},
	{306, 88,319, 98, PTB_EO_EXIT}
};

const guiButton_t bSampler[] =
{
	{  6,124, 25,134, PTB_SA_EXIT},
	{  0,138,319,201, PTB_SA_SAMPLEAREA},
	{  3,205,316,210, PTB_SA_ZOOMBARAREA},

	{ 32,211, 95,221, PTB_SA_PLAYWAVE},
	{ 96,211,175,221, PTB_SA_SHOWRANGE},
	{176,211,245,221, PTB_SA_ZOOMOUT},

	{  0,222, 30,243, PTB_SA_STOP},
	{ 32,222, 95,232, PTB_SA_PLAYDISPLAY},
	{ 96,222,175,232, PTB_SA_SHOWALL},
	{176,222,245,232, PTB_SA_RANGEALL},
	{246,222,319,232, PTB_SA_LOOP},

	{ 32,233, 94,243, PTB_SA_PLAYRANGE},
	{ 96,233,115,243, PTB_SA_RANGEBEG},
	{116,233,135,243, PTB_SA_RANGEEND},
	{136,233,174,243, PTB_SA_RANGECENTER},
	{176,233,210,243, PTB_SA_SAMPLE},
	{211,233,245,243, PTB_SA_RESAMPLE},
	{246,233,319,243, PTB_SA_RESAMPLENOTE},

	{  0,244, 31,254, PTB_SA_CUT},
	{ 32,244, 63,254, PTB_SA_COPY},
	{ 64,244, 95,254, PTB_SA_PASTE},
	{ 96,244,135,254, PTB_SA_VOLUME},
	{136,244,175,254, PTB_SA_TUNETONE},
	{176,244,210,254, PTB_SA_FIXDC},
	{211,244,245,254, PTB_SA_FILTERS}
};
