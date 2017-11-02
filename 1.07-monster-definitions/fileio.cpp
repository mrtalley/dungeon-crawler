#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>
#include <stdlib.h>

#include "fileio.h"
#include "dungeon.h"

using namespace std;

std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(0, s.find_first_not_of(t));
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

void parse_monster_name(npc_type *monst, string line)
{
    monst->name = line.substr(5);
}

void parse_monster_desc(npc_type *monst, ifstream *f)
{
    string line;

    getline(*f, line);
    monst->desc.append(trim(line));

    while(line.compare(".") != 0) {
        getline(*f, line);
        monst->desc.append(trim(line));
    }

    // This is ugly and inefficient, but it's late and I'm tired
    monst->desc = monst->desc.substr(0, monst->desc.length() - 1);
}

int str_to_int_color(string string_color)
{
    int tc = 0;

    if(!string_color.compare("BLACK")) tc = COLOR_BLACK;
    else if(!string_color.compare("RED")) tc = COLOR_RED;
    else if(!string_color.compare("GREEN")) tc = COLOR_GREEN;
    else if(!string_color.compare("YELLOW")) tc = COLOR_YELLOW;
    else if(!string_color.compare("BLUE")) tc = COLOR_BLUE;
    else if(!string_color.compare("MAGENTA")) tc = COLOR_MAGENTA;
    else if(!string_color.compare("CYAN")) tc = COLOR_CYAN;
    else if(!string_color.compare("WHITE")) tc = COLOR_WHITE;

    return tc;
}

string int_to_str_color(int color)
{
    switch(color) {
        case 0:
            return "BLACK";
        case 1:
            return "RED";
        case 2:
            return "GREEN";
        case 3:
            return "YELLOW";
        case 4:
            return "BLUE";
        case 5:
            return "MAGENTA";
        case 6:
            return "CYAN";
        case 7:
            return "WHITE";
        default:
            break;
    }
    return NULL;
}

void parse_color(npc_type *monst, string line)
{
    string color = line.substr(6);
    string sub_color = "";
    while(color.compare("")) {
        if(color.find(" ", 0) == string::npos) {
            monst->color.push_back(str_to_int_color(color));
            color = "";
        } else {
            sub_color = color.substr(0, color.find(" "));
            monst->color.push_back(str_to_int_color(sub_color));
            color.erase(0, color.find(" ") + 1);
        }
    }
}

void parse_symbol(npc_type *monst, string line)
{
    monst->symb = line.substr(5)[0];
}

void parse_speed(npc_type *monst, string line)
{
    string speed = line.substr(6);
    string sub_speed;

    sub_speed = speed.substr(0, speed.find("+"));
    monst->speed[base] = atoi(sub_speed.c_str());
    speed.erase(0, speed.find("+") + 1);

    sub_speed = speed.substr(0, speed.find("d"));
    monst->speed[dice] = atoi(sub_speed.c_str());
    speed.erase(0, speed.find("d") + 1);

    monst->speed[sides] = atoi(speed.c_str());
}

void parse_dam(npc_type *monst, string line)
{
    string dam = line.substr(4);
    string sub_dam;

    sub_dam = dam.substr(0, dam.find("+"));
    monst->dam[base] = atoi(sub_dam.c_str());
    dam.erase(0, dam.find("+") + 1);

    sub_dam = dam.substr(0, dam.find("d"));
    monst->dam[dice] = atoi(sub_dam.c_str());
    dam.erase(0, dam.find("d") + 1);

    monst->dam[sides] = atoi(dam.c_str());
}

void parse_hp(npc_type *monst, string line)
{
    string hp = line.substr(3);
    string sub_hp;

    sub_hp = hp.substr(0, hp.find("+"));
    monst->hp[base] = atoi(sub_hp.c_str());
    hp.erase(0, hp.find("+") + 1);

    sub_hp = hp.substr(0, hp.find("d"));
    monst->hp[dice] = atoi(sub_hp.c_str());
    hp.erase(0, hp.find("d") + 1);

    monst->hp[sides] = atoi(hp.c_str());
}

int str_to_int_abil(string abil)
{
    int new_abil = 0;

    if(!abil.compare("SMART")) new_abil = NPC_SMART;
    else if(!abil.compare("TELE")) new_abil = NPC_TELEPATH;
    else if(!abil.compare("TUNNEL")) new_abil = NPC_TUNNEL;
    else if(!abil.compare("ERRATIC")) new_abil = NPC_ERRATIC;
    else if(!abil.compare("PASS")) new_abil = NPC_PASS;
    else if(!abil.compare("PICKUP")) new_abil = NPC_PICKUP;
    else if(!abil.compare("DESTROY")) new_abil = NPC_DESTROY;
    else if(!abil.compare("UNIQ")) new_abil = NPC_UNIQ;

    return new_abil;
}

string int_to_str_abil(int abil)
{
    switch(abil) {
        case NPC_SMART:
            return "SMART";
        case NPC_TELEPATH:
            return "TELE";
        case NPC_TUNNEL:
            return "TUNNEL";
        case NPC_ERRATIC:
            return "ERRATIC";
        case NPC_PASS:
            return "PASS";
        case NPC_PICKUP:
            return "PICKUP";
        case NPC_DESTROY:
            return "DESTROY";
        case NPC_UNIQ:
            return "UNIQ";
        default:
            break;
    }

    return NULL;
}

void parse_abil(npc_type *monst, string line)
{
    string abil = line.substr(5);
    string sub_abil = "";
    while(abil.compare("")) {
        if(abil.find(" ", 0) == string::npos) {
            monst->abil.push_back(str_to_int_abil(abil));
            abil = "";
        } else {
            sub_abil = abil.substr(0, abil.find(" "));
            monst->abil.push_back(str_to_int_abil(sub_abil));
            abil.erase(0, abil.find(" ") + 1);
        }
    }
}

void parse_monster_file(dungeon_t *d)
{
    string home = getenv("HOME");
    string meta;
    string meta_actual = "RLG327 MONSTER DESCRIPTION 1";
    string begin = "BEGIN MONSTER";
    string name_actual = "NAME";
    string desc_actual = "DESC";
    string symb_actual = "SYMB";
    string color_actual = "COLOR";
    string speed_actual = "SPEED";
    string abil_actual = "ABIL";
    string hp_actual = "HP";
    string dam_actual = "DAM";
    string end = "END";

    ifstream f(home + "/.rlg327/monster_desc.txt");
    string line;
    npc_type *new_monster;

    /* Get first line in file */
    getline(f, meta);

    /* If first line isn't the expected meta, exit */
    if(meta.compare(meta_actual) != 0) {
        return;
    }

    f.get();

    while(!f.eof()) {
        getline(f, line);

        /* Begin new monster info */
        if(!line.compare(begin)) {
            new_monster = new npc_type();
            continue;
        }

        /* Parse name of monster */
        if(line.find(name_actual) != string::npos) {
            parse_monster_name(new_monster, line);
            continue;
        }

        /* Parse description of monster */
        /* TODO: Consider parsing this better -- adding spaces *
         * where necessary and whatnot.                        */
        /* TODO: Parse last period out -- don't want to        *
         * include it.                                         */
        if(line.find(desc_actual) != string::npos) {
            parse_monster_desc(new_monster, &f);
            continue;
        }

        /* Parse color(s) */
        if(line.find(color_actual) != string::npos) {
            parse_color(new_monster, line);
            continue;
        }

        /* Parse symbol */
        if(line.find(symb_actual) != string::npos) {
            parse_symbol(new_monster, line);
            continue;
        }

        /* Parse speed */
        if(line.find(speed_actual) != string::npos) {
            parse_speed(new_monster, line);
            continue;
        }

        /* Parse damage */
        if(line.find(dam_actual) != string::npos) {
            parse_dam(new_monster, line);
            continue;
        }

        /* Parse hp */
        if(line.find(hp_actual) != string::npos) {
            parse_hp(new_monster, line);
            continue;
        }

        /* Parse hp */
        if(line.find(abil_actual) != string::npos) {
            parse_abil(new_monster, line);
            continue;
        }

        /* End of monster, push onto list */
        if(!line.compare(end)) {
            d->monst_desc.push_back(*new_monster);
        }
    }

    f.close();
}

void print_monster_descriptions(dungeon_t *d)
{
    npc_type monster;

    for(int i = 0; i < d->monst_desc.size(); i++) {
        monster = d->monst_desc.at(i);
        cout << monster.name << endl;
        cout << monster.desc << endl;
        cout << monster.symb << endl;

        for(int j = 0; j < monster.color.size(); j++) {
            cout << int_to_str_color(monster.color.at(j)) << " ";
        }
        cout << endl;

        cout << monster.speed[base] << "+";
        cout << monster.speed[dice] << "d";
        cout << monster.speed[sides] << endl;

        for(int j = 0; j < monster.abil.size(); j++) {
            cout << int_to_str_abil(monster.abil.at(j)) << " ";
        }
        cout << endl;

        cout << monster.hp[base] << "+";
        cout << monster.hp[dice] << "d";
        cout << monster.hp[sides] << endl;

        cout << monster.dam[base] << "+";
        cout << monster.dam[dice] << "d";
        cout << monster.dam[sides] << endl;
        cout << endl;
    }
}
