#include <string>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <limits.h>
#include <ncurses.h>
#include <vector>
#include <sstream>
#include <cstdlib>

#include "descriptions.h"
#include "dungeon.h"
#include "npc.h"
#include "dice.h"
#include "character.h"
#include "utils.h"
#include "event.h"

#define MONSTER_FILE_SEMANTIC          "RLG327 MONSTER DESCRIPTION"
#define MONSTER_FILE_VERSION           1U
#define NUM_MONSTER_DESCRIPTION_FIELDS 8
#define OBJECT_FILE_SEMANTIC           "RLG327 OBJECT DESCRIPTION"
#define OBJECT_FILE_VERSION            1U
#define NUM_OBJECT_DESCRIPTION_FIELDS  12

static const struct {
  const char *name;
  const uint32_t value;
} abilities_lookup[] = {
  /* There are only 32 bits available.  So far we're only using five *
   * of them (but even if we were using all 32, that's a very small  *
   * number), so it's much more efficient to do a linear search      *
   * rather than a binary search.  Zeros on the end are sentinals to *
   * stop the search.  Two notes: 1) Performance isn't a big deal    *
   * here, since this is initialization, not gameplay; and           *
   * 2) Alphabetizing these just because.                            */
  { "DESTROY", NPC_DESTROY_OBJ },
  { "ERRATIC", NPC_ERRATIC     },
  { "PASS",    NPC_PASS_WALL   },
  { "PICKUP",  NPC_PICKUP_OBJ  },
  { "SMART",   NPC_SMART       },
  { "TELE",    NPC_TELEPATH    },
  { "TUNNEL",  NPC_TUNNEL      },
  { "UNIQ",    NPC_UNIQ        },
  { 0,         0               }
};

#define color_lu_entry(color) { #color, COLOR_##color }
static const struct {
  const char *name;
  const uint32_t value;
} colors_lookup[] = {
  /* Same deal here as above in abilities_lookup definition. */
  /* We can use this convenient macro here, but we can't use a *
   * similar macro above because of PASS and TELE.             */
  /* color_lu_entry(BLACK), Can't display COLOR_BLACK */
  "BLACK", COLOR_WHITE,
  color_lu_entry(BLUE),
  color_lu_entry(CYAN),
  color_lu_entry(GREEN),
  color_lu_entry(MAGENTA),
  color_lu_entry(RED),
  color_lu_entry(WHITE),
  color_lu_entry(YELLOW),
  { 0, 0 }
};

#define type_lu_entry(type) { #type, objtype_##type }
static const struct {
  const char *name;
  const object_type_t value;
} types_lookup[] = {
  type_lu_entry(WEAPON),
  type_lu_entry(OFFHAND),
  type_lu_entry(RANGED),
  type_lu_entry(LIGHT),
  type_lu_entry(ARMOR),
  type_lu_entry(HELMET),
  type_lu_entry(CLOAK),
  type_lu_entry(GLOVES),
  type_lu_entry(BOOTS),
  type_lu_entry(AMULET),
  type_lu_entry(RING),
  type_lu_entry(SCROLL),
  type_lu_entry(BOOK),
  type_lu_entry(FLASK),
  type_lu_entry(GOLD),
  type_lu_entry(AMMUNITION),
  type_lu_entry(FOOD),
  type_lu_entry(WAND),
  type_lu_entry(CONTAINER),
  { 0, objtype_no_type }
};

const char object_symbol[] = {
  '*',  /* objtype_no_type    */
  '|',  /* objtype_WEAPON     */
  ')',  /* objtype_OFFHAND    */
  '}',  /* objtype_RANGED     */
  '~',  /* objtype_LIGHT      */
  '[',  /* objtype_ARMOR      */
  ']',  /* objtype_HELMET     */
  '(',  /* objtype_CLOAK      */
  '{',  /* objtype_GLOVES     */
  '\\', /* objtype_BOOTS      */
  '"',  /* objtype_AMULET     */
  '=',  /* objtype_RING       */
  '`',  /* objtype_SCROLL     */
  '?',  /* objtype_BOOK       */
  '!',  /* objtype_FLASK      */
  '$',  /* objtype_GOLD       */
  '/',  /* objtype_AMMUNITION */
  ',',  /* objtype_FOOD       */
  '-',  /* objtype_WAND       */
  '%',  /* objtype_CONTAINER  */
};

static inline void eat_whitespace(std::ifstream &f)
{
  while (isspace(f.peek())) {
    f.get();
  }  
}

static inline void eat_blankspace(std::ifstream &f)
{
  while (isblank(f.peek())) {
    f.get();
  }  
}

static uint32_t parse_name(std::ifstream &f,
                           std::string *lookahead,
                           std::string *name)
{
  /* Always start by eating the blanks.  If we then find a newline, we *
   * know there's an error in the file.  If we eat all whitespace,     *
   * we'd consume newlines and perhaps miss a restart on the next      *
   * line.                                                             */

  eat_blankspace(f);

  if (f.peek() == '\n') {
    return 1;
  }

  getline(f, *name);

  /* We enter this function with the semantic in the lookahead, so we  *
   * read a new one so that we're in the same state for the next call. */
  f >> *lookahead;

  return 0;
}

static uint32_t parse_monster_name(std::ifstream &f,
                                   std::string *lookahead,
                                   std::string *name)
{
  return parse_name(f, lookahead, name);
}

static uint32_t parse_monster_symb(std::ifstream &f,
                                   std::string *lookahead,
                                   char *symb)
{
  eat_blankspace(f);

  if (f.peek() == '\n') {
    return 1;
  }

  *symb = f.get();

  eat_blankspace(f);
  if (f.peek() != '\n') {
    return 1;
  }

  f >> *lookahead;

  return 0;
}

static uint32_t parse_color(std::ifstream &f,
                            std::string *lookahead,
                            uint32_t *color)
{
  uint32_t i;

  *color = UINT_MAX;

  eat_blankspace(f);

  if (f.peek() == '\n') {
    return 1;
  }

  f >> *lookahead;

  for (i = 0; colors_lookup[i].name; i++) {
    if (*lookahead == colors_lookup[i].name) {
      *color = colors_lookup[i].value;
      break;
    }
  }

  if (!colors_lookup[i].name) {
    return 1;
  }

  eat_blankspace(f);
  if (f.peek() != '\n') {
    return 1;
  }

  f >> *lookahead;

  return 0;
}

static uint32_t parse_monster_color(std::ifstream &f,
                                    std::string *lookahead,
                                    std::vector<uint32_t> *color)
{
  uint32_t i;
  uint32_t c;

  c = UINT_MAX;

  eat_blankspace(f);

  if (f.peek() == '\n') {
    return 1;
  }

  do {
    f >> *lookahead;

    for (i = 0; colors_lookup[i].name; i++) {
      if (*lookahead == colors_lookup[i].name) {
        c = colors_lookup[i].value;
        break;
      }
    }

    if (!colors_lookup[i].name) {
      return 1;
    }

    color->push_back(c);

    eat_blankspace(f);
  } while (f.peek() != '\n');

  f >> *lookahead;

  return 0;
}

static uint32_t parse_desc(std::ifstream &f,
                           std::string *lookahead,
                           std::string *desc)
{
  /* DESC is special.  Data doesn't follow on the same line *
   * as the keyword, so we want to eat the newline, too.    */
  eat_blankspace(f);

  if (f.peek() != '\n') {
    return 1;
  }

  f.get();

  while (f.peek() != EOF) {
    getline(f, *lookahead);
    if (lookahead->length() > 77) {
      return 1;
    }

    lookahead->push_back('\n');

    if (*lookahead == ".\n") {
      break;
    }

    *desc += *lookahead;
  }

  /* Strip off the trailing newline */
  desc->erase(desc->length() - 1);

  if (*lookahead != ".\n") {
    return 1;
  }

  f >> *lookahead;

  return 0;
}

static uint32_t parse_monster_desc(std::ifstream &f,
                                   std::string *lookahead,
                                   std::string *desc)
{
  return parse_desc(f, lookahead, desc);
}

typedef uint32_t (*dice_parser_func_t)(std::ifstream &f,
                                       std::string *lookahead,
                                       dice *hit);

static uint32_t parse_dice(std::ifstream &f,
                           std::string *lookahead,
                           dice *d)
{
  int32_t base;
  uint32_t number, sides;

  eat_blankspace(f);

  if (f.peek() == '\n') {
    return 1;
  }

  f >> *lookahead;

  if (sscanf(lookahead->c_str(), "%d+%ud%u", &base, &number, &sides) != 3) {
    return 1;
  }

  d->set(base, number, sides);

  f >> *lookahead;

  return 0;
}

static dice_parser_func_t parse_monster_speed = parse_dice;
static dice_parser_func_t parse_monster_dam = parse_dice;
static dice_parser_func_t parse_monster_hp = parse_dice;

static uint32_t parse_monster_abil(std::ifstream &f,
                                   std::string *lookahead,
                                   uint32_t *abil)
{
  uint32_t i;

  *abil = 0;

  eat_blankspace(f);

  if (f.peek() == '\n') {
    return 1;
  }

  /* Will not lead to error if an ability is listed multiple times. */
  while (f.peek() != '\n') {
    f >> *lookahead;

    for (i = 0; abilities_lookup[i].name; i++) {
      if (*lookahead == abilities_lookup[i].name) {
        *abil |= abilities_lookup[i].value;
        break;
      }
    }

    if (!abilities_lookup[i].name) {
      return 1;
    }

    eat_blankspace(f);
  }

  f >> *lookahead;

  return 0;
}

static uint32_t parse_monster_description(std::ifstream &f,
                                          std::string *lookahead,
                                          std::vector<monster_description> *v)
{
  std::string s;
  bool read_name, read_symb, read_color, read_desc,
       read_speed, read_dam, read_hp, read_abil;
  std::string name, desc;
  char symb;
  uint32_t abil;
  std::vector<uint32_t> color;
  dice speed, dam, hp;
  monster_description m;
  int count;

  read_name = read_symb = read_color = read_desc =
              read_speed = read_dam = read_hp = read_abil = false;

  if (*lookahead != "BEGIN") {
    std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
              << "Parse error in monster description.\n"
              << "Discarding monster." << std::endl;
    do {
      f >> *lookahead;
    } while (*lookahead != "BEGIN" && f.peek() != EOF);
  }
  if (f.peek() == EOF) {
    return 1;
  }
  f >> *lookahead;
  if (*lookahead != "MONSTER") {
    return 1;
  }

  for (f >> *lookahead, count = 0;
       count < NUM_MONSTER_DESCRIPTION_FIELDS;
       count++) {
    /* This could definately be more concise. */
    if        (*lookahead == "NAME")  {
      if (read_name || parse_monster_name(f, lookahead, &name)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in monster name.\n"
                  << "Discarding monster." << std::endl;
        return 1;
      }
      read_name = true;
    } else if (*lookahead == "DESC")  {
      if (read_desc || parse_monster_desc(f, lookahead, &desc)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in monster description.\n"
                  << "Discarding monster." << std::endl;
        return 1;
      }
      read_desc = true;
    } else if (*lookahead == "SYMB")  {
      if (read_symb || parse_monster_symb(f, lookahead, &symb)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in monster symbol.\n"
                  << "Discarding monster." << std::endl;
        return 1;
      }
      read_symb = true;
    } else if (*lookahead == "COLOR") {
      if (read_color || parse_monster_color(f, lookahead, &color)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in monster color.\n"
                  << "Discarding monster." << std::endl;
        return 1;
      }
      read_color = true;
    } else if (*lookahead == "SPEED") {
      if (read_speed || parse_monster_speed(f, lookahead, &speed)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in monster speed.\n"
                  << "Discarding monster." << std::endl;
        return 1;
      }
      read_speed = true;
    } else if (*lookahead == "ABIL")  {
      if (read_abil || parse_monster_abil(f, lookahead, &abil)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in monster abilities.\n"
                  << "Discarding monster." << std::endl;
        return 1;
      }
      read_abil = true;
    } else if (*lookahead == "HP")    {
      if (read_hp || parse_monster_hp(f, lookahead, &hp)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in monster hitpoints.\n"
                  << "Discarding monster." << std::endl;
        return 1;
      }
      read_hp = true;
    } else if (*lookahead == "DAM")   {
      if (read_dam || parse_monster_dam(f, lookahead, &dam)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in monster damage.\n"
                  << "Discarding monster." << std::endl;
        return 1;
      }
      read_dam = true;
    } else                           {
      std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                << "Parse error in monster description.\n"
                << "Discarding monster." << std::endl;
      return 1;
    }
  }

  if (*lookahead != "END") {
    return 1;
  }

  eat_blankspace(f);
  if (f.peek() != '\n' && f.peek() != EOF) {
    return 1;
  }
  f >> *lookahead;

  m.set(name, desc, symb, color, speed, abil, hp, dam);
  v->push_back(m);

  return 0;
}

static uint32_t parse_object_name(std::ifstream &f,
                                  std::string *lookahead,
                                  std::string *name)
{

  return parse_name(f, lookahead, name);
}

static uint32_t parse_object_desc(std::ifstream &f,
                                  std::string *lookahead,
                                  std::string *desc)
{
  return parse_desc(f, lookahead, desc);
}

static uint32_t parse_object_type(std::ifstream &f,
                                  std::string *lookahead,
                                  object_type_t *type)
{
  uint32_t i;

  *type = objtype_no_type;

  eat_blankspace(f);

  if (f.peek() == '\n') {
    return 1;
  }

  f >> *lookahead;

  for (i = 0; types_lookup[i].name; i++) {
    if (*lookahead == types_lookup[i].name) {
      *type = types_lookup[i].value;
      break;
    }
  }

  if (*type == objtype_no_type) {
    return 1;
  }

  eat_blankspace(f);
  if (f.peek() != '\n') {
    return 1;
  }

  f >> *lookahead;

  return 0;
}

static uint32_t parse_object_color(std::ifstream &f,
                                   std::string *lookahead,
                                   uint32_t *color)
{
  return parse_color(f, lookahead, color);
}

static dice_parser_func_t parse_object_hit = parse_dice;
static dice_parser_func_t parse_object_dam = parse_dice;
static dice_parser_func_t parse_object_dodge = parse_dice;
static dice_parser_func_t parse_object_def = parse_dice;
static dice_parser_func_t parse_object_weight = parse_dice;
static dice_parser_func_t parse_object_speed = parse_dice;
static dice_parser_func_t parse_object_attr = parse_dice;
static dice_parser_func_t parse_object_val = parse_dice;

static uint32_t parse_object_description(std::ifstream &f,
                                         std::string *lookahead,
                                         std::vector<object_description> *v)
{
  std::string s;
  bool read_name, read_desc, read_type, read_color,
       read_hit, read_dam, read_dodge, read_def,
       read_weight, read_speed, read_attr, read_val;
  std::string name, desc;
  uint32_t color;
  object_type_t type;
  dice hit, dam, dodge, def, weight, speed, attr, val;
  object_description o;
  int count;

  read_name = read_desc = read_type = read_color =
              read_hit = read_dam = read_dodge = read_def =
              read_weight = read_speed = read_attr = read_val = false;

  if (*lookahead != "BEGIN") {
    std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
              << "Parse error in object description.\n"
              << "Discarding object." << std::endl;
    do {
      f >> *lookahead;
    } while (*lookahead != "BEGIN" && f.peek() != EOF);
  }
  if (f.peek() == EOF) {
    return 1;
  }
  f >> *lookahead;
  if (*lookahead != "OBJECT") {
    return 1;
  }

  for (f >> *lookahead, count = 0;
       count < NUM_OBJECT_DESCRIPTION_FIELDS;
       count++) {
    /* This could definately be more concise. */
    if        (*lookahead == "NAME")  {
      if (read_name || parse_object_name(f, lookahead, &name)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object name.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_name = true;
    } else if (*lookahead == "DESC")  {
      if (read_desc || parse_object_desc(f, lookahead, &desc)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object description.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_desc = true;
    } else if (*lookahead == "TYPE")  {
      if (read_type || parse_object_type(f, lookahead, &type)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object type.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_type = true;
    } else if (*lookahead == "COLOR") {
      if (read_color || parse_object_color(f, lookahead, &color)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object color.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_color = true;
    } else if (*lookahead == "HIT")   {
      if (read_hit || parse_object_hit(f, lookahead, &hit)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object hit bonux.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_hit = true;
    } else if (*lookahead == "DAM")   {
      if (read_dam || parse_object_dam(f, lookahead, &dam)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object damage bonus.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_dam = true;
    } else if (*lookahead == "DODGE")   {
      if (read_dodge || parse_object_dodge(f, lookahead, &dodge)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object dodge bonus.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_dodge = true;
    } else if (*lookahead == "DEF")   {
      if (read_def || parse_object_def(f, lookahead, &def)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object defence bonus.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_def = true;
    } else if (*lookahead == "WEIGHT")   {
      if (read_weight || parse_object_weight(f, lookahead, &weight)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object weight.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_weight = true;
    } else if (*lookahead == "SPEED") {
      if (read_speed || parse_object_speed(f, lookahead, &speed)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object speed bonus.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_speed = true;
    } else if (*lookahead == "ATTR")  {
      if (read_attr || parse_object_attr(f, lookahead, &attr)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object special attribute bonus.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_attr = true;
    } else if (*lookahead == "VAL")    {
      if (read_val || parse_object_val(f, lookahead, &val)) {
        std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                  << "Parse error in object value.\n"
                  << "Discarding object." << std::endl;
        return 1;
      }
      read_val = true;
    } else                           {
      std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
                << "Parse error in object description.\n"
                << "Discarding object." << std::endl;
      return 1;
    }
  }

  if (*lookahead != "END") {
    return 1;
  }

  eat_blankspace(f);
  if (f.peek() != '\n' && f.peek() != EOF) {
    return 1;
  }
  f >> *lookahead;

  o.set(name, desc, type, color, hit, dam,
        dodge, def, weight, speed, attr, val);
  v->push_back(o);

  return 0;
}

static uint32_t parse_monster_descriptions(std::ifstream &f,
                                           dungeon *d,
                                           std::vector<monster_description> *v)
{
  std::string s;
  std::stringstream expected;
  std::string lookahead;

  expected << MONSTER_FILE_SEMANTIC << " " << MONSTER_FILE_VERSION;

  eat_whitespace(f);

  getline(f, s);

  if (s != expected.str()) {
    std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
              << "Parse error in monster description file.\nExpected: \""
              << expected.str() << "\"\nRead:     \"" << s << "\"\n\nAborting."
              << std::endl;
    return 1;
  }

  f >> lookahead;
  do {
    parse_monster_description(f, &lookahead, v);
  } while (f.peek() != EOF);

  return 0;
}

static uint32_t parse_object_descriptions(std::ifstream &f,
                                          dungeon *d,
                                          std::vector<object_description> *v)
{
  std::string s;
  std::stringstream expected;
  std::string lookahead;

  expected << OBJECT_FILE_SEMANTIC << " " << OBJECT_FILE_VERSION;

  eat_whitespace(f);

  getline(f, s);

  if (s != expected.str()) {
    std::cerr << "Discovered at " << __FILE__ << ":" << __LINE__ << "\n"
              << "Parse error in object description file.\nExpected: \""
              << expected.str() << "\"\nRead:     \"" << s << "\"\n\nAborting."
              << std::endl;
    return 1;
  }

  f >> lookahead;
  do {
    parse_object_description(f, &lookahead, v);
  } while (f.peek() != EOF);

  return 0;
}

uint32_t parse_descriptions(dungeon *d)
{
  std::string file;
  std::ifstream f;
  uint32_t retval;

  retval = 0;

  file = getenv("HOME");
  if (file.length() == 0) {
    file = ".";
  }
  file += std::string("/") + SAVE_DIR + "/" + MONSTER_DESC_FILE;

  f.open(file.c_str());

  if (parse_monster_descriptions(f, d, &d->monster_descriptions)) {
    retval = 1;
  }

  f.close();

  file = getenv("HOME");
  if (file.length() == 0) {
    file = ".";
  }
  file += std::string("/") + SAVE_DIR + "/" + OBJECT_DESC_FILE;

  f.open(file.c_str());

  if (parse_object_descriptions(f, d, &d->object_descriptions)) {
    retval = 1;
  }

  f.close();

  return retval;
}

uint32_t print_descriptions(dungeon *d)
{
  std::vector<monster_description> &m = d->monster_descriptions;
  std::vector<monster_description>::iterator mi;
  std::vector<object_description> &o = d->object_descriptions;
  std::vector<object_description>::iterator oi;

  for (mi = m.begin(); mi != m.end(); mi++) {
    std::cout << *mi << std::endl;
  }


  for (oi = o.begin(); oi != o.end(); oi++) {
    std::cout << *oi << std::endl;
  }

  return 0;
}

void monster_description::set(const std::string &name,
                              const std::string &description,
                              const char symbol,
                              const std::vector<uint32_t> &color,
                              const dice &speed,
                              const uint32_t abilities,
                              const dice &hitpoints,
                              const dice &damage)
{
  this->name = name;
  this->description = description;
  this->symbol = symbol;
  this->color = color;
  this->speed = speed;
  this->abilities = abilities;
  this->hitpoints = hitpoints;
  this->damage = damage;
}

std::ostream &monster_description::print(std::ostream& o)
{
  uint32_t i;
  uint32_t num_abilities;
  std::vector<uint32_t>::iterator ci;

  o << name << std::endl;
  o << description << std::endl;
  o << symbol << std::endl;
  for (ci = color.begin(); ci != color.end(); ci++) {
    for (i = 0; colors_lookup[i].name; i++) {
      if (*ci == colors_lookup[i].value) {
        o << colors_lookup[i].name << " ";
        break;
      }
    }
  }
  o << std::endl;
  o << speed << std::endl;
  for (i = num_abilities = 0; abilities_lookup[i].name; i++) {
    if (abilities & abilities_lookup[i].value) {
      num_abilities++;
      o << abilities_lookup[i].name << " ";
    }
  }

  if (!num_abilities) {
    o << "Monster failing to live up to full potential";
  }

  return o << std::endl << hitpoints << std::endl << damage << std::endl;
}

std::ostream &operator<<(std::ostream &o, monster_description &m)
{
  return m.print(o);
}

uint32_t destroy_descriptions(dungeon *d)
{
  d->monster_descriptions.clear();
  d->object_descriptions.clear();

  return 0;
}

void object_description::set(const std::string &name,
                             const std::string &description,
                             const object_type_t type,
                             const uint32_t color,
                             const dice &hit,
                             const dice &damage,
                             const dice &dodge,
                             const dice &defence,
                             const dice &weight,
                             const dice &speed,
                             const dice &attrubute,
                             const dice &value)
{
  this->name = name;
  this->description = description;
  this->type = type;
  this->color = color;
  this->hit = hit;
  this->damage = damage;
  this->dodge = dodge;
  this->defence = defence;
  this->weight = weight;
  this->speed = speed;
  this->attribute = attrubute;
  this->value = value;
}

std::ostream &object_description::print(std::ostream &o)
{
  uint32_t i;

  o << name << std::endl;
  o << description << std::endl;
  for (i = 0; types_lookup[i].name; i++) {
    if (type == types_lookup[i].value) {
      o << types_lookup[i].name << std::endl;
      break;
    }
  }
  for (i = 0; colors_lookup[i].name; i++) {
    if (color == colors_lookup[i].value) {
      o << colors_lookup[i].name << std::endl;
      break;
    }
  }

return o << hit << std::endl << damage << std::endl << dodge << std::endl
         << defence << std::endl << weight << std::endl << speed << std::endl
         << attribute << std::endl << value << std::endl;
}

std::ostream &operator<<(std::ostream &o, object_description &od)
{
  return od.print(o);
}

npc *monster_description::generate_monster(dungeon *d)
{
  npc *n;
  std::vector<monster_description> &v = d->monster_descriptions;
  uint32_t i;

  while (!v[(i = rand_range(0, v.size() - 1))].can_be_generated())
    ;

  monster_description &m = v[i];

  n = new npc(d, m);

  heap_insert(&d->events, new_event(d, event_character_turn, n, 0));

  return n;
}
