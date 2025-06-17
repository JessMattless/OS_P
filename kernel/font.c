#include "font.h"

// Font is mindset
// https://int10h.org/oldschool-pc-fonts/fontlist/font?mindset#-
unsigned long long font[256] = {
    0x0000000000000000, // 0x00 Null character
    0x7E8199BD81A5817E, // 0x01 Start of Heading
    0x7EFFE7C3FFDBFF7E, // 0x02 Start of Text
    0x00081C3E7F7F7F36, // 0x03 End of Text
    0x00081C3E7F3E1C08, // 0x04 End of Transmission
    0x3E1C3E7F7F1C3E1C, // 0x05 Enquiry
    0x3E1C3E7F3E1C0808, // 0x06 Acknowledge
    0x0000183C3C180000, // 0x07 Bell, Alert
    0xFFFFE7C3C3E7FFFF, // 0x08 Backspace
    0x003C664242663C00, // 0x09 Horizontal Tab
    0x0000000000000000, // 0x0A Line Feed
    0x1E333333BEF0E0F0, // 0x0B Vertical Tabulation
    0x187E183C6666663C, // 0x0C Form Feed
    0x070F0E0C0CFCCCFC, // 0x0D Carriage Return
    0x0367E6C6C6FEC6FE, // 0x0E Shift Out
    0x995A3CE7E73C5A99, // 0x0F Shift In
    0x0001071F7F1F0701, // 0x10 Data Link Escape
    0x0040707C7F7C7040, // 0x11 Device Control One (XON)
    0x0C1E3F0C0C3F1E0C, // 0x12 Device Control Two
    0x0033003333333333, // 0x13 Device Control Three (XOFF)
    0x00D8D8D8DEDBDBFE, // 0x14 Device Control Four
    0x1E331C36361CC67C, // 0x15 Negative Acknowledge
    0x007E7E7E00000000, // 0x16 Synchronous Idle
    0xFF183C7E187E3C18, // 0x17 End of Transmission Block
    0x00181818187E3C18, // 0x18 Cancel
    0x00183C7E18181818, // 0x19 End of medium
    0x00003060FE603000, // 0x1A Substitute
    0x0000180CFE0C1800, // 0x1B Escape
    0x00007F0303030000, // 0x1C File Separator
    0x00002466FF662400, // 0x1D Group Separator
    0x0000FFFF7E3C1800, // 0x1E Record Separator
    0x0000183C7EFFFF00, // 0x1F Unit Separator
    0x0000000000000000, // 0x20 Space
    0x000C000C0C1E1E0C, // 0x21 Exclamation mark
    0x0000000000363636, // 0x22 Double quotes (or speech marks)
    0x0036367F367F3636, // 0x23 Number sign
    0x000C1F301E033E0C, // 0x24 Dollar
    0x0063660C18336300, // 0x25 Per cent sign
    0x006E333B6E1C361C, // 0x26 Ampersand
    0x0000000000030606, // 0x27 Single quote
    0x000C06030303060C, // 0x28 Open parenthesis (or open bracket)
    0x0003060C0C0C0603, // 0x29 Close parenthesis (or close bracket)
    0x0000663CFF3C6600, // 0x2A Asterisk
    0x00000C0C3F0C0C00, // 0x2B Plus
    0x060C0C0000000000, // 0x2C Comma
    0x000000003F000000, // 0x2D Hyphen-minus
    0x000C0C0000000000, // 0x2E Period, dot or full stop
    0x000103060C183060, // 0x2F Slash or divide
    0x003E676F7B73633E, // 0x30 Zero
    0x003F0C0C0C0C0E0C, // 0x31 One
    0x003F33061C30331E, // 0x32 Two
    0x001E33301C30331E, // 0x33 Three
    0x0078307F33363C38, // 0x34 Four
    0x001E3330301F033F, // 0x35 Five
    0x001E33331F03061C, // 0x36 Six
    0x000C0C0C1830333F, // 0x37 Seven
    0x001E33331E33331E, // 0x38 Eight
    0x000E18303E33331E, // 0x39 Nine
    0x000C0C00000C0C00, // 0x3A Colon
    0x060C0C00000C0C00, // 0x3B Semicolon
    0x00180C0603060C18, // 0x3C Less than (or open angled bracket)
    0x00003F00003F0000, // 0x3D Equals
    0x0003060C180C0603, // 0x3E Greater than (or close angled bracket)
    0x000C000C1830331E, // 0x3F Question mark
    0x001E037B7B7B633E, // 0x40 At sign
    0x0033333F33331E0C, // 0x41 Uppercase A
    0x003F66663E66663F, // 0x42 Uppercase B
    0x003C66030303663C, // 0x43 Uppercase C
    0x001F36666666361F, // 0x44 Uppercase D
    0x007F46161E16467F, // 0x45 Uppercase E
    0x000F06161E16467F, // 0x46 Uppercase F
    0x007C66730303663C, // 0x47 Uppercase G
    0x003333333F333333, // 0x48 Uppercase H
    0x000F06060606060F, // 0x49 Uppercase I
    0x001E333330303078, // 0x4A Uppercase J
    0x006766361E366667, // 0x4B Uppercase K
    0x007F66460606060F, // 0x4C Uppercase L
    0x0063636B7F7F7763, // 0x4D Uppercase M
    0x006363737B6F6763, // 0x4E Uppercase N
    0x001C36636363361C, // 0x4F Uppercase O
    0x000F06063E66663F, // 0x50 Uppercase P
    0x00381E3B3333331E, // 0x51 Uppercase Q
    0x006766363E66663F, // 0x52 Uppercase R
    0x001E33381E07331E, // 0x53 Uppercase S
    0x001E0C0C0C0C2D3F, // 0x54 Uppercase T
    0x003F333333333333, // 0x55 Uppercase U
    0x000C1E3333333333, // 0x56 Uppercase V
    0x0063777F6B636363, // 0x57 Uppercase W
    0x0063361C1C366363, // 0x58 Uppercase X
    0x001E0C0C1E333333, // 0x59 Uppercase Y
    0x007F664C1831637F, // 0x5A Uppercase Z
    0x001E06060606061E, // 0x5B Opening bracket
    0x00406030180C0603, // 0x5C Backslash
    0x001E18181818181E, // 0x5D Closing bracket
    0x0000000063361C08, // 0x5E Caret - circumflex
    0xFF00000000000000, // 0x5F Underscore
    0x0000000000180C0C, // 0x60 Grave accent
    0x006E333E301E0000, // 0x61 Lowercase a
    0x003B66666E360607, // 0x62 Lowercase b
    0x001E3303331E0000, // 0x63 Lowercase c
    0x006E33333B363038, // 0x64 Lowercase d
    0x001E033F331E0000, // 0x65 Lowercase e
    0x000F06060F06361C, // 0x66 Lowercase f
    0x1F303E33336E0000, // 0x67 Lowercase g
    0x006766666E360607, // 0x68 Lowercase h
    0x001E0C0C0C0E000C, // 0x69 Lowercase i
    0x1E33333030380030, // 0x6A Lowercase j
    0x0067361E36660607, // 0x6B Lowercase k
    0x001E0C0C0C0C0C0E, // 0x6C Lowercase l
    0x00636B7F7F330000, // 0x6D Lowercase m
    0x006666666E3B0000, // 0x6E Lowercase n
    0x001E3333331E0000, // 0x6F Lowercase o
    0x0F063E66663B0000, // 0x70 Lowercase p
    0x78303E33336E0000, // 0x71 Lowercase q
    0x000F06666E3B0000, // 0x72 Lowercase r
    0x001F301E033E0000, // 0x73 Lowercase s
    0x00182C0C0C3E0C08, // 0x74 Lowercase t
    0x006E333333330000, // 0x75 Lowercase u
    0x000C1E3333330000, // 0x76 Lowercase v
    0x00367F7F6B630000, // 0x77 Lowercase w
    0x0063361C36630000, // 0x78 Lowercase x
    0x1F303E3333330000, // 0x79 Lowercase y
    0x003F260C193F0000, // 0x7A Lowercase z
    0x00380C0C070C0C38, // 0x7B Opening brace
    0x0018181818181818, // 0x7C Vertical bar
    0x00070C0C380C0C07, // 0x7D Closing brace
    0x0000000000003B6E, // 0x7E Equivalency sign - tilde
    0x0018181800181818, // 0x7F Delete/Vertical Line

    0x38203C660303663C, // 0x80 
    0x006E333333330012, // 0x81 Latin Lowercase u with dieresis
    0x003C067E663D0204, // 0x82 Latin Lowercase e with acute
    0x006E333E301E211E, // 0x83 Latin Lowercase a with circumflex
    0x006E333E301E0012, // 0x84 Latin Lowercase a with dieresis
    0x006E333E305E2010, // 0x85 Latin Lowercase a with grave
    0x006E333E301E000C, // 0x86 Latin Lowercase a with macron
    0x1C103C0603063C00, // 0x87 
    0x001E033F331E211E, // 0x88 Latin Lowercase e with circumflex
    0x001E033F331E0012, // 0x89 Latin Lowercase e with dieresis
    0x001E033F335E2010, // 0x8A Latin Lowercase e with grave
    0x001E0C0C0C0E0012, // 0x8B Latin Lowercase i with dieresis
    0x001E0C0C0C0E211E, // 0x8C Latin Lowercase i with circumflex
    0x001E0C0C0C2E100C, // 0x8D Latin Lowercase i with grave
    0x00637F63773E0822, // 0x8E Latin Uppercase A with dieresis
    0x00637F63773E0008, // 0x8F Latin Uppercase A with ring
    0x007C0C3C0C7D0204, // 0x90 Latin Uppercase E with acute
    0x003E097E48360000, // 0x91 Latin Lowercase Ligature ae
    0x007909097F090B7E, // 0x92 Latin Uppercase Ligature AE
    0x001E3333331E211E, // 0x93 Latin Lowercase o with circumflex
    0x001E3333331E0012, // 0x94 Latin Lowercase o with dieresis
    0x001E3333335E2010, // 0x95 Latin Lowercase o with grave
    0x006E33333300211E, // 0x96 Latin Lowercase u with circumflex
    0x006E333333402010, // 0x97 Latin Lowercase u with grave
    0x1F303E3333330012, // 0x98 Latin Lowercase y with dieresis
    0x001C366363361C22, // 0x99 Latin Uppercase O with dieresis
    0x003E636363630022, // 0x9A Latin Uppercase U with dieresis
    0x003C66031F03663C, // 0x9B Euro Symbol
    0x003F66060F26361C, // 0x9C Pound Symbol
    0x08087F007F142241, // 0x9D Yen Symbol
    0x0061217127290907, // 0x9E 
    0x00060D0C1E0C2C18, // 0x9F Function Symbol
    0x006E333E301D0204, // 0xA0 Latin Lowercase a with acute
    0x003C1818181D021C, // 0xA1 Latin Lowercase i with acute
    0x001C366363361D02, // 0xA2 Latin Lowercase o with acute
    0x006E333333010204, // 0xA3 Latin Lowercase u with acute
    0x003333331F001926, // 0xA4 Latin Lowercase n with tilde
    0x0066767E6E661966, // 0xA5 Latin Uppercase N with tilde
    0x7F006E333E301E00, // 0xA6 
    0x3F001E3333331E00, // 0xA7 
    0x001E3303060C000C, // 0xA8 Upside Down Question Mark
    0x00000202027E0000, // 0xA9 Inverted Negation Symbol
    0x00004040407E0000, // 0xAA Negation Symbol
    0x7021427405091121, // 0xAB 1/2
    0x4071525405091121, // 0xAC 1/4
    0x000C1E1E0C0C000C, // 0xAD Upside Down Exclamation Mark
    0x0000442211224400, // 0xAE Double Left Chevron
    0x0000112244221100, // 0xAF Double Right Chevron
    0x1144228811442288, // 0xB0 Thin Particle
    0x55AA55AA55AA55AA, // 0xB1 Medium Particle
    0xEEBBDD77EEBBDD77, // 0xB2 Thick Particle
    0x0808080808080808, // 0xB3 Vertical Line (Thin)
    0x080808080F080808, // 0xB4 T-Junction Left (Thin)
    0x0808080F080F0808, // 0xB5 T-Junction Left (Thin-Double)
    0x1414141417141414, // 0xB6 T-Junction Left (Double-Thin)
    0x141414141F000000, // 0xB7 Top-Right Corner (Thin-Double)
    0x0808080F080F0000, // 0xB8 Top-Right Corner (Double-Thin)
    0x1414141710171414, // 0xB9 T-Junction Left (Double)
    0x1414141414141414, // 0xBA Vertical Line (Double)
    0x14141417101F0000, // 0xBB Top-Right Corner (Double)
    0x0000001F10171414, // 0xBC Bottom-Right Corner (Double)
    0x000000001F141414, // 0xBD Bottom-Right Corner (Thin-Double)
    0x0000000F080F0808, // 0xBE Bottom-Right Corner (Double-Thin)
    0x080808080F000000, // 0xBF Top-Right Corner (Thin)
    0x00000000F0101010, // 0xC0 Bottom-Left Corner (Thin)
    0x00000000FF080808, // 0xC1 T-Junction Up (Thin)
    0x08080808FF000000, // 0xC2 T-Junction Down (Thin)
    0x08080808F8080808, // 0xC3 T-Junction Right (Thin)
    0x00000000FF000000, // 0xC4 Horizontal Line (Thin)
    0x08080808FF080808, // 0xC5 Cross (Thin)
    0x080808F808F80808, // 0xC6 T-Junction Right (Thin-Double)
    0x14141414F4141414, // 0xC7 T-Junction Right (Double-Thin)
    0x000000FC04F41414, // 0xC8 Bottom-Left Corner (Double)
    0x141414F404FC0000, // 0xC9 Top-Left Corner (Double)
    0x000000FF00F71414, // 0xCA T-Junction Up (Double)
    0x141414F700FF0000, // 0xCB T-Junction Down (Double)
    0x141414F404F41414, // 0xCC T-Junction Right (Double)
    0x000000FF00FF0000, // 0xCD Horizontal Line (Double)
    0x141414F700F71414, // 0xCE Cross (Double)
    0x000000FF00FF0808, // 0xCF T-Junction Up (Double-Thin)
    0x00000000FF141414, // 0xD0 T-Junction Up (Thin-Double)
    0x080808FF00FF0000, // 0xD1 T-Junction Down (Double-Thin)
    0x14141414FF000000, // 0xD2 T-Junction Down (Thin-Double)
    0x00000000FC141414, // 0xD3 Bottom-Left Corner (Thin-Double)
    0x000000F808F80808, // 0xD4 Bottom-Left Corner (Double-Thin)
    0x080808F808F80000, // 0xD5 Top-Left Corner (Double-Thin)
    0x14141414FC000000, // 0xD6 Top-Left Corner (Thin-Double)
    0x14141414FF141414, // 0xD7 Cross (Thin-Double)
    0x0808087F087F0808, // 0xD8 Cross (Double-Thin)
    0x000000000F080808, // 0xD9 Bottom-Right Corner (Thin)
    0x08080808F8000000, // 0xDA Top-Right Corner (Thin)
    0xFFFFFFFFFFFFFFFF, // 0xDB Thick Wall - Square
    0xFFFFFFFF00000000, // 0xDC Thick Wall - Bottom
    0x0F0F0F0F0F0F0F0F, // 0xDD Thick Wall - Left
    0xF0F0F0F0F0F0F0F0, // 0xDE Thick Wall - Right
    0x00000000FFFFFFFF, // 0xDF Thick Wall - Top
    0x0066190919660000, // 0xE0 Lowercase Alpha
    0x031F33331F33321C, // 0xE1 lowercase Beta
    0x001C36361C366300, // 0xE2 
    0x00242424257F0000, // 0xE3 Lowercase Pi
    0x007F43060C06437F, // 0xE4 Uppercase Sigma
    0x000E1B1B0E7C0000, // 0xE5 Lowercase Sigma
    0x0101016D33333300, // 0xE6 Lowercase Mu
    0x0018181818191B7E, // 0xE7 Uppercase Tau
    0x1C083E6B6B3E081C, // 0xE8 Uppercase Phi
    0x001C36227F22361C, // 0xE9 Uppercase Theta
    0x007736634163361C, // 0xEA Uppercase Omega
    0x0E1B1B0E0301331E, // 0xEB 
    0x0022554955220000, // 0xEC 
    0x02043E6B6B3E1020, // 0xED 
    0x007C06037F03067C, // 0xEE 
    0x004141414163361C, // 0xEF 
    0x00003F003F003F00, // 0xF0 Exact Equals Symbol
    0x003F000C0C3F0C0C, // 0xF1 Plus-Minus Symbol
    0x003F003F180C0603, // 0xF2 More Than or Equals Symbol
    0x003F003F060C1830, // 0xF3 Less Than or Equals Symbol
    0x0C0C0C0C2C2C3800, // 0xF4 Upside Down Hook
    0x00070D0D0C0C0C0C, // 0xF5 Hook
    0x00000C003F000C00, // 0xF6 Division Symbol
    0x00003B6E003B6E00, // 0xF7 Double Tilde
    0x000000000C12120C, // 0xF8 
    0x0000001818000000, // 0xF9 Point
    0x0000000018000000, // 0xFA Short Hyphen
    0x00040E0B08780000, // 0xFB Square Root Symbol
    0x000000202222361D, // 0xFC 
    0x00001F030E181B0E, // 0xFD 
    0x003C3C3C3C3C0000, // 0xFE Small Box
    0x0000000000000000, // 0xFF Unused
};