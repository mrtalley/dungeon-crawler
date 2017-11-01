#include <iostream>
#include <fstream>
#include <sstream> // string stream
#include <string>

#include "fileio.h"
#include "dungeon.h"

using namespace std;

void parse_monster_file(dungeon_t *d)
{
    string home = getenv("HOME");
    string meta;
    string meta_actual = "RLG327 MONSTER DESCRIPTION 1";
    string begin = "BEGIN MONSTER";
    string name_actual = "NAME";
    string desc_actual = "DESC";
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
    cout << meta << endl;

    /* If first line isn't the expected meta, exit */
    if(meta.compare(meta_actual) != 0) {
        return;
    }

    // cout << (char) f.peek() << endl;
    f.get(); // skip newline

    while(!f.eof()) {
        getline(f, line);

        /* Begin new monster info */
        if(!line.compare(begin)) {
            new_monster = new npc_type();
            continue;
        }

        /* Parse name of monster */
        if(line.find(name_actual) != string::npos) {
            new_monster->name = line.substr(5, sizeof(line));
            cout << new_monster->name << endl;
            continue;
        }

        if(line.find(desc_actual) != string::npos) {
            getline(f, line);
            new_monster->desc.append(line);

            while(line.compare(".") != 0) {
                new_monster->desc.append(line);
                getline(f, line);
            }
            cout << new_monster->desc << endl;
            cout << endl;
        }

        /* End of monster, push onto list */
        if(!line.compare(end)) {
            d->monst_desc.push_back(*new_monster);
        }
    }

    f.close();
}
