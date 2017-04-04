#ifndef HASH_H
#define HASH_H

#include <QtGlobal>

constexpr quint8 shuffle[] = {
    0x84, 0xE1, 0xE4, 0x7E, 0x42, 0xBA, 0x59, 0x48, 0xF0, 0x6C, 0x5C, 0x19, 0x7A, 0xE2, 0x1A, 0x5D,
    0x5A, 0xD4, 0x33, 0xC5, 0xAC, 0x04, 0xED, 0xF9, 0xD3, 0x3C, 0xBF, 0x23, 0xDC, 0xB7, 0xE9, 0x4B,
    0xCD, 0x0A, 0x9D, 0x47, 0x75, 0xD9, 0x3A, 0xE7, 0x10, 0x03, 0xE0, 0xF1, 0xAA, 0x99, 0x9F, 0xB6,
    0x64, 0x2B, 0xA2, 0x9B, 0x0C, 0x78, 0x18, 0x3E, 0xD8, 0x43, 0xE5, 0x08, 0x34, 0x88, 0xAE, 0x85,
    0xEA, 0x58, 0x3D, 0x2F, 0xAD, 0x35, 0x8A, 0x7D, 0x2D, 0xBE, 0x22, 0x38, 0x65, 0x98, 0xDB, 0xFA,
    0xA9, 0x26, 0x37, 0x13, 0x7F, 0xB2, 0x79, 0x1C, 0xDA, 0x66, 0x8E, 0xC1, 0xB0, 0x14, 0xEC, 0xC4,
    0x36, 0x6A, 0xCF, 0x4C, 0x7C, 0xFF, 0x20, 0xDD, 0x05, 0x31, 0xF8, 0x8F, 0xF5, 0x27, 0x44, 0x3F,
    0x5E, 0x1E, 0x62, 0x25, 0x06, 0x49, 0xF2, 0x3B, 0x67, 0xFC, 0xCC, 0x95, 0xFE, 0xBB, 0x24, 0x80,
    0x76, 0xF4, 0x8B, 0x40, 0xC0, 0x55, 0xC7, 0xD6, 0x4F, 0xB5, 0x71, 0xDE, 0x60, 0x86, 0xB4, 0xC6,
    0x69, 0x53, 0xE8, 0xCA, 0xFD, 0x9A, 0x2E, 0x5B, 0xA3, 0x41, 0x28, 0xC2, 0xD5, 0xF6, 0x87, 0xD0,
    0xA6, 0xEE, 0x45, 0x07, 0xBD, 0x01, 0x17, 0x89, 0xC9, 0x9C, 0x52, 0x81, 0x6D, 0xFB, 0x0D, 0x1F,
    0x94, 0xE6, 0x1D, 0xD2, 0x09, 0x56, 0x91, 0x5F, 0xDF, 0xCE, 0x0F, 0xF3, 0x0B, 0x70, 0x39, 0x97,
    0x15, 0xC8, 0x4A, 0x2A, 0x2C, 0x21, 0x90, 0x96, 0xE3, 0x6F, 0x72, 0xB1, 0x77, 0x8D, 0xA8, 0x0E,
    0x74, 0x32, 0x57, 0xD7, 0x93, 0xB9, 0x11, 0xCB, 0xAF, 0x6E, 0xA7, 0x61, 0xA0, 0x51, 0xA4, 0x92,
    0x68, 0xAB, 0x02, 0xC3, 0x4D, 0x46, 0x16, 0x82, 0x7B, 0xA5, 0x54, 0x6B, 0x1B, 0xF7, 0xD1, 0x29,
    0xB3, 0xEF, 0xA1, 0xEB, 0xB8, 0x9E, 0x12, 0x50, 0x30, 0x00, 0x8C, 0xBC, 0x83, 0x4E, 0x63, 0x73
};

constexpr size_t primes[] = {
    0x000003E5, 0x000F2AD9, 0x3B11DF1D, 0x0C99ECD7, 0x13776184, 0xCFEEC75F, 0xCCEE7824, 0x1CB9E752,
    0xE001E2C9, 0x67583C35, 0x7AA27BFB, 0x9ACCDA64, 0xDFCE89CE, 0x9F5EB2AD, 0xABCDDE2D, 0x18C247DE,
    0x6CA1E3F6, 0x127CCEB5, 0x00110731, 0x425101D5, 0x457C238B, 0x9C766D65, 0x59380CBA, 0x774991BD,
    0x918496E1, 0xB95F9C7B, 0xF15C6DD8, 0xFCF7CDE3, 0x3114D8E8, 0x2630C047, 0xBBDCD517, 0xA309E56E,
    0xF58A87E0, 0x44832F1C, 0xD2E67916, 0x5B9595E3, 0xAD90BE73, 0xF4B5B882, 0x07B79603, 0x0DFB39CD,
    0x73681B97, 0x747374D4, 0x85A5FF69, 0x7F7BB5F5, 0x7CCBA519, 0x0519FC43, 0xDE337102, 0x5E57202B,
    0x695048E6, 0x25ABE958, 0xB683C44A, 0xCF2B76F8, 0xD44656FE, 0xB5F0CE70, 0x92D3FCF4, 0xD398247F,
    0x0F8625D3, 0x75714EFB, 0x62489950, 0xC4BD160E, 0x3466E784, 0x14C3A5E0, 0xDDF501B0, 0x6B2F95D0,
    0x705274B1, 0x7120770A, 0x936F9BAA, 0x31A93F50, 0x68239351, 0x928CBC0A, 0xBE18552C, 0x54C3B740,
    0x1E38AD8A, 0xB2BBDAE7, 0x159B885B, 0x26BA0ABB, 0xD28BCADD, 0xFA6D11E4, 0x4AC6B0C3, 0x37CE6892,
    0x56DD4173, 0x4BAFE631, 0xC40B7DFB, 0x80C1A582, 0x7229953F, 0x9BF24017, 0x56739BF2, 0xB03E56CA,
    0x62C80360, 0xB4F52660, 0xBEBE76A0, 0xDBC40006, 0xE2541AB5, 0x718C065A, 0x3654BE3C, 0x9808E07F,
    0x1A9250EB, 0x7BD5239E, 0x4513B838, 0x05CC7325, 0x953C6F2F, 0x345D0450, 0xEE41CC5B, 0xE640E206,
    0xBAB044DE, 0x107C376D, 0x33C3DBC1, 0x99C6D76E, 0xE36501BC, 0x985FC4A1, 0x6CF8C956, 0x64E81D96,
    0xFBFB3AB6, 0x596BAAA3, 0x404F8E2B, 0x75D4AE71, 0xE54B5FDF, 0xFE8C62F7, 0x58BD6FD2, 0x99C47E33,
    0xDA3F7EF5, 0xF949737A, 0xDB0EBDEC, 0x2069AB71, 0x3B88AF93, 0xDB53C866, 0x2D4B7894, 0x66EC9914,
    0xD7702C74, 0x07DD230B, 0xA03979F5, 0xFFD7F998, 0x641F10DC, 0xECFCAA51, 0xF403510F, 0x50EAB321,
    0x220BA0C0, 0x97490C44, 0x2F7CC721, 0xF0F3843D, 0x6462053B, 0xF1BE604D, 0x7A6D0F8E, 0xCABD95E2,
    0x9458BC3F, 0xBD95239C, 0x55D3B16E, 0x417202B4, 0xE1048802, 0x56A5B336, 0x7352F29F, 0x220AE6FC,
    0x947593F0, 0x2DE927F2, 0xCD08922C, 0x8261487A, 0xC4DF451D, 0xB9882CEF, 0x8F57019D, 0x3DD94A9F,
    0xDF3F9E2B, 0x72C300DC, 0xF1725A8A, 0x525A9F1E, 0xBAEDB116, 0xFFB2AD85, 0xD2DDCADC, 0x39C70E01,
    0x04398AC6, 0x7419752E, 0x27255DEA, 0x7486C0EA, 0xD0CD5117, 0x2F9CD1C0, 0x6DBCE179, 0x609A1DE8,
    0x38367A00, 0xEC2922DA, 0xBC34BE99, 0xF96A4CB9, 0x5AFCD048, 0x5A9729CA, 0xCEB5C112, 0x09D8EE3F,
    0x59D7DB81, 0xE5A9DEC2, 0x6E908D08, 0x98F541D6, 0xB32968C1, 0xC044FA5E, 0xCCA31302, 0xF71909E6,
    0x54839080, 0x2461C3C9, 0xB0BF7E5A, 0x59C71732, 0xA45D5717, 0x1F842F13, 0xBDCB5579, 0x28E3E320,
    0x3F838C3F, 0x5B513252, 0xA338FABD, 0xACE8848C, 0x658C37DD, 0x7B15913C, 0x5AFEA08B, 0x61A73EB9,
    0x505747F9, 0xE3EB4DF5, 0xA3669EA0, 0x5EA7C79C, 0xA36C63FC, 0x752166E8, 0x2B15C750, 0xCBD13B37,
    0xC5DBA04C, 0x90574AFE, 0x23F71168, 0x1136CA94, 0x0A62F2A7, 0x735B048B, 0x4178B318, 0xFB117D76,
    0xCB1DA05F, 0x0A619512, 0x6E098F42, 0x8B3AEDB6, 0x3C7FC7EC, 0x9DA59B07, 0xF5F4C4A8, 0xE241E605,
    0x2AA4D4EA, 0x13F133F8, 0xAA5F6525, 0x8584EBB0, 0xFEA9E677, 0xCBAE9152, 0x3EDBF773, 0xCEAAB3D3,
    0xDECE57E3, 0xB99C4A72, 0xDDADF0CC, 0x566ACDDB, 0x8DF3B737, 0xD6288B5B, 0x0BE6BCA9, 0x599CBE5B,
    0xFF7159C3, 0xD4729851, 0x624B36B0, 0xCEEBFCEE, 0xDD100E0B, 0xEF86B433, 0xD79BCE43, 0xB1CA4E36
};

constexpr size_t hash(const char *str, size_t i = 0) {
    return (str[i]
            ? shuffle[static_cast<quint8>(str[i])] * primes[i] + hash(str, i+1)
            : 0);
}

constexpr size_t operator "" _h(const char *str, size_t) {
    return hash(str);
}


#endif // HASH_H
