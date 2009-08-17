// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "TLDList.h"


QMap<QString, QString>* TLDList::TLDMap;

TLDList::TLDList( QObject* parent) : QObject(parent){
    if ( TLDMap == NULL ) {
        TLDMap = new QMap<QString, QString>;
        QString TLDString = tr("AC:Ascension Island\n"
                "AD:Andorra\n"
                "AE:United Arab Emirates\n"
                "AERO:Aircraft-related\n"
                "AF:Afghanistan\n"
                "AG:Antigua and Barbuda\n"
                "AI:Anguilla\n"
                "AL:Albania\n"
                "AM:Armenia\n"
                "AN:Netherland Antilles\n"
                "AO:Angola\n"
                "AQ:Antarctica\n"
                "AR:Argentina\n"
                "ARPA:Address and Routing Parameter Area\n"
                "AS:American Samoa\n"
                "AT:Austria\n"
                "AU:Australia\n"
                "AW:Aruba\n"
                "AZ:Azerbaijan\n"
                "BA:Bosnia-Herzegovina\n"
                "BB:Barbados\n"
                "BE:Belgium\n"
                "BF:Burkina Faso\n"
                "BG:Bulgaria\n"
                "BH:Bahrain\n"
                "BI:Burundi\n"
                "BIz:Business\n"
                "BJ:Benin\n"
                "BM:Bermuda\n"
                "BN:Brunei Darussalam\n"
                "BO:Bolivia\n"
                "BR:Brazil\n"
                "BS:Bahamas\n"
                "BT:Bhutan\n"
                "BW:Botswana\n"
                "BY:Belarus\n"
                "BZ:Belize\n"
                "CA:Canada\n"
                "CC:Cocos (Keeling) Islands\n"
                "CD:Democratic Republic of Congo\n"
                "CF:Central African Republic\n"
                "CG:Congo\n"
                "CH:Switzerland\n"
                "CI:Ivory Coast\n"
                "CK:Cook Islands\n"
                "CL:Chile\n"
                "CM:Cameroon\n"
                "CN:China\n"
                "CO:Colombia\n"
                "COM:Commercial\n"
                "COOP:Cooperative-related\n"
                "CR:Costa Rica\n"
                "CU:Cuba\n"
                "CV:Cape Verde\n"
                "CX:Christmas Island\n"
                "CY:Cyprus\n"
                "CZ:Czech Republic\n"
                "DE:Germany\n"
                "DJ:Djibouti\n"
                "DK:Denmark\n"
                "DM:Dominica\n"
                "DO:Dominican Republic\n"
                "DZ:Algeria\n"
                "EC:Ecuador\n"
                "EDU:Educational\n"
                "EE:Estonia\n"
                "EG:Egypt\n"
                "ES:Spain\n"
                "ET:Ethiopia\n"
                "FI:Finland\n"
                "FJ:Fiji\n"
                "FK:Falkland Islands (Malvinas)\n"
                "FM:Micronesia\n"
                "FO:Faroe Islands\n"
                "FR:France\n"
                "GB:Great Britan\n"
                "GA:Gabon\n"
                "GD:Grenada\n"
                "GE:Georgia\n"
                "GF:French Guyana\n"
                "GH:Ghana\n"
                "GI:Gibraltar\n"
                "GL:Greenland\n"
                "GM:Gambia\n"
                "GN:Guinea\n"
                "GOV:Government\n"
                "GP:Guadeloupe (French)\n"
                "GQ:Equatorial Guinea\n"
                "GR:Greece\n"
                "GT:Guatemala\n"
                "GU:Guam (US)\n"
                "GY:Guyana\n"
                "HK:Hong Kong\n"
                "HM:Heard and McDonald Islands\n"
                "HN:Honduras\n"
                "HR:Croatia (Hrvatska)\n"
                "HU:Hungary\n"
                "ID:Indonesia\n"
                "IE:Ireland\n"
                "IL:Israel\n"
                "IN:India\n"
                "INFO:General-purpose TLD\n"
                "INT:International\n"
                "IO:British Indian Ocean Territory\n"
                "IR:Islamic Republic of Iran\n"
                "IS:Iceland\n"
                "IT:Italy\n"
                "JM:Jamaica\n"
                "JO:Jordan\n"
                "JP:Japan\n"
                "KE:Kenya\n"
                "KG:Kyrgyzstan\n"
                "KH:Cambodia\n"
                "KI:Kiribati\n"
                "KM:Comoros\n"
                "KN:Saint Kitts Nevis Anguilla\n"
                "KR:South Korea\n"
                "KW:Kuwait\n"
                "KY:Cayman Islands\n"
                "KZ:Kazakhstan\n"
                "LA:Laos (People's Democratic Republic)\n"
                "LB:Lebanon\n"
                "LC:Saint Lucia\n"
                "LI:Liechtenstein\n"
                "LK:Sri Lanka\n"
                "LR:Liberia\n"
                "LS:Lesotho\n"
                "LT:Lithuania\n"
                "LU:Luxembourg\n"
                "LV:Latvia\n"
                "LY:Libya (Libyan Arab Jamahiriya)\n"
                "MA:Morocco\n"
                "MC:Monaco\n"
                "MD:Moldavia\n"
                "MG:Madagascar\n"
                "MH:Marshall Islands\n"
                "MIL:US Military\n"
                "ML:Mali\n"
                "MM:Myanmar\n"
                "MN:Mongolia\n"
                "MO:Macau\n"
                "MP:Northern Mariana Islands\n"
                "MQ:Martinique (French)\n"
                "MR:Mauritania\n"
                "MS:Montserrat\n"
                "MT:Malta\n"
                "MU:Mauritius\n"
                "MUseum:Museum-related\n"
                "MV:Maldives\n"
                "MW:Malawi\n"
                "MX:Mexico\n"
                "MY:Malaysia\n"
                "MZ:Mozambique\n"
                "NA:Namibia\n"
                "NAME:Personal name\n"
                "NC:New Caledonia (French)\n"
                "NE:Niger\n"
                "NET:Network Infrastructure\n"
                "NF:Norfolk Island\n"
                "NG:Nigeria\n"
                "NI:Nicaragua\n"
                "NL:Netherlands\n"
                "NO:Norway\n"
                "NP:Nepal\n"
                "NR:Nauru\n"
                "NU:Niue\n"
                "NZ:New Zealand\n"
                "OM:Oman\n"
                "ORG:Nonprofit\n"
                "PA:Panama\n"
                "PE:Peru\n"
                "PF:French Polynesia\n"
                "PF:Polynesia (French)\n"
                "PG:Papua New Guinea\n"
                "PH:Philippines\n"
                "PK:Pakistan\n"
                "PL:Poland\n"
                "PM:Saint Pierre and Miquelon\n"
                "PN:Pitcairn\n"
                "PR:Puerto Rico (US)\n"
                "PRo:Professional domain\n"
                "PS:Palestina\n"
                "PT:Portugal\n"
                "PW:Palau\n"
                "PY:Paraguay\n"
                "QA:Qatar\n"
                "RE:Reunion (French)\n"
                "RO:Romania\n"
                "RU:Russian Federation\n"
                "RW:Rwanda\n"
                "SA:Saudi Arabia\n"
                "SB:Solomon Islands\n"
                "SC:Seychelles\n"
                "SE:Sweden\n"
                "SG:Singapore\n"
                "SH:Saint Helena\n"
                "SI:Slovenia\n"
                "SK:Slovak Republic (Slovakia)\n"
                "SL:Sierra Leone\n"
                "SM:San Marino\n"
                "SN:Senegal\n"
                "SO:Somalia\n"
                "SR:Surinam\n"
                "ST:Saint Tome and Principe\n"
                "SU:Soviet Union\n"
                "SV:El Salvador\n"
                "SZ:Swaziland\n"
                "TC:Turks and Caicos Islands\n"
                "TD:Chad\n"
                "TF:French Southern Territories\n"
                "TG:Togo\n"
                "TH:Thailand\n"
                "TJ:Tajikistan\n"
                "TK:Tokelau\n"
                "TM:Turkmenistan\n"
                "TN:Tunisia\n"
                "TO:Tonga\n"
                "TP:East Timor\n"
                "TR:Turkey\n"
                "TT:Trinidad and Tobago\n"
                "TV:Tuvalu\n"
                "TW:Taiwan\n"
                "TZ:Tanzania\n"
                "UA:Ukraine\n"
                "UG:Uganda\n"
                "UK:United Kingdom\n"
                "US:United States of America\n"
                "UY:Uruguay\n"
                "UZ:Uzbekistan\n"
                "VA:Vatican City State\n"
                "VC:Saint Vincent and the Grenadines\n"
                "VE:Venezuela\n"
                "VG:Virgin Islands (British)\n"
                "VI:Virgin Islands (US)\n"
                "VN:Vietnam\n"
                "VU:Vanuatu\n"
                "WS:Samoa\n"
                "YE:Yemen\n"
                "YU:Yugoslavia\n"
                "ZA:South Africa\n"
                "ZM:Zambia\n"
                "ZR:Zaire\n"
                "ZW:Zimbabwe\n"
                "XX:?\n"
                );
        QStringList list = TLDString.split( "\n" );
        foreach( QString s, list ) {
            TLDMap->insert( s.section( ":", 0, 0 ), s.section( ":", 1, 1 ) );
        }
    }
}

TLDList::~TLDList() {
}
