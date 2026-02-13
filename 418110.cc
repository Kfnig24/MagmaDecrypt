// Nom: MagmaDecrypt
// Autheur: Kélian Fnighar
// Version: 1
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Caractères acceptés
const string ACCEPTED_CHAR("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMN"
                           "OPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}");
const string ACCEPTED_KEY("FGIKMNX[^lm\\012RSacqrdgtvxy");

// Error messages
const string
    BAD_SYMBOL_NB("Error: the symbol set size must be in the range ]2, 64]");
const string WRONG_SYMBOL_NB("Error: the number of symbols "
                             "must match the symbol set size");

const string
    WRONG_SYMBOL_VALUE("Error: at least one symbol value is not allowed");
const string DUPLICATED_SYMBOL_VALUE("Error: all symbols must be different");

const string WRONG_COMPL_SYMBOL_NB("Error: the number of complementary symbols "
                                   "is incorrect");

const string WRONG_COMPL_SYMBOL_VALUE("Error: complementary symbols "
                                      "must be part of the symbol set");

const string BAD_GRID_SIZE("Error: the grid size must be in the range [2, 64]");
const string WRONG_GRID_LINE_SIZE("Error: the number of symbols in each line "
                                  "must match the number of columns");

const string WRONG_GRID_SYMBOL_VALUE("Error: all grid symbols "
                                     "must be part of the symbol set");

const string BAD_KEY_VALUE("Error: the key value is invalid");

// Final message
const string THE_END("The end");

void print_error(string message);

string get_caracteres(unsigned int nbS);
string get_complement(unsigned int nbS, const string &caracteres);
vector<string> get_grille(unsigned nbL, unsigned nbC, const string &caracteres);
void lecture(unsigned int &nbS, string &caracteres, string &caract_compl,
             unsigned int &nbL, unsigned int &nbC, vector<string> &grille,
             char &cle);

vector<string> create_table(const string &caracteres,
                            const string &caract_compl, unsigned int nbS);
void affichage(const vector<string> &table, const string &caracteres,
               const vector<string> &grille);

char mult(char a, char b, const vector<string> &table, string &caracteres);
char mult_grille(char a, char b, char c, char d, const vector<string> &table,
                 char cle, string &caracteres);
vector<string> decryptage(const vector<string> &table,
                          const vector<string> &grille, unsigned int nbL,
                          unsigned int nbC, char cle, string &caracteres);

int main() {
  unsigned int nbS(0);
  string caracteres;
  string caract_compl;
  vector<string> table;
  unsigned int nbL(0);
  unsigned int nbC(0);

  vector<string> grille;
  char cle;

  lecture(nbS, caracteres, caract_compl, nbL, nbC, grille, cle);
  table = create_table(caracteres, caract_compl, nbS);

  affichage(table, caracteres, grille);
  cout << endl;

  decryptage(table, grille, nbL, nbC, cle, caracteres);

  cout << THE_END << endl;

  return 0;
}


void print_error(string message) {
  cout << message;
  cout << endl;
  exit(0);
}


string get_caracteres(unsigned int nbS) {
  string resp;
  cin >> resp;

  if (resp.size() != nbS)
    print_error(WRONG_SYMBOL_NB);

  unsigned int i(0);
  for (char c : resp) {
    if (ACCEPTED_CHAR.find(c) == string::npos)
      print_error(WRONG_SYMBOL_VALUE);

    string original = resp;
    string popped = original.replace(i, 1, "");
    if (popped.find(c) != string::npos)
      print_error(DUPLICATED_SYMBOL_VALUE);

    ++i;
  }

  return resp;
}

string get_complement(unsigned int nbS, const string &caracteres) {
  int nba = (nbS + 2 * (nbS - 1));
  int nbt = (nbS * nbS - nba) / 2;
  string resp;
  cin >> resp;

  if (resp.size() != nbt)
    print_error(WRONG_COMPL_SYMBOL_NB);

  for (char c : resp) {
    if (caracteres.find(c) == string::npos)
      print_error(WRONG_COMPL_SYMBOL_VALUE);
  }

  return resp;
}

vector<string> get_grille(unsigned nbL, unsigned nbC,
                          const string &caracteres) {
  vector<string> resp;

  for (int i(1); i <= nbL; ++i) {
    string ligne;
    cin >> ligne;

    if (ligne.size() != nbC)
      print_error(WRONG_GRID_LINE_SIZE);

    for (char c : ligne) {
      if (caracteres.find(c) == string::npos)
        print_error(WRONG_GRID_SYMBOL_VALUE);
    }

    resp.push_back(ligne);
  }

  return resp;
}

void lecture(unsigned int &nbS, string &caracteres, string &caract_compl,
             unsigned int &nbL, unsigned int &nbC, vector<string> &grille,
             char &cle) {

  cin >> nbS;
  if ((nbS <= 2) || (nbS > 64))
    print_error(BAD_SYMBOL_NB);

  caracteres = get_caracteres(nbS);

  caract_compl = get_complement(nbS, caracteres);

  cin >> nbL;
  cin >> nbC;
  if ((nbC < 2) || (nbC > 64) || (nbL < 2) || (nbL > 64))
    print_error(BAD_GRID_SIZE);

  grille = get_grille(nbL, nbC, caracteres);

  cin >> cle;
  if (ACCEPTED_KEY.find(cle) == string::npos)
    print_error(BAD_KEY_VALUE);
}


vector<string> create_table(const string &caracteres,
                            const string &caract_compl, unsigned int nbS) {
  vector<string> table;

  int compl_iterator_apres_idem(0);
  for (int i(1); i <= nbS; ++i) {
    if (i == 1) {
      table.push_back(caracteres);
      continue;
    }

    string line;
    bool idempotence_passe = false;
    for (int j(1); j <= nbS; ++j) {
      if (j == 1) {
        line.push_back(caracteres.at(i - 1));
        continue;
      }
      if (i == j) {
        line.push_back(caracteres.at(i - 1));
        idempotence_passe = true;
        continue;
      }

      if (idempotence_passe) {
        line.push_back(caract_compl.at(compl_iterator_apres_idem));
        ++compl_iterator_apres_idem;
      } else {
        line.push_back(table.at(j - 1).at(i - 1));
      }
    }
    table.push_back(line);
  }

  return table;
}

void affichage(const vector<string> &table, const string &caracteres,
               const vector<string> &grille) {

  cout << "  " << caracteres << endl;
  for (int i(0); i < caracteres.size(); ++i) {
    cout << caracteres.at(i) << " " << table.at(i) << endl;
  }

  cout << endl;

  for (string ligne : grille) {
    cout << ligne << endl;
  }
}


char mult(char a, char b, const vector<string> &table, string &caracteres) {
  int i_a = caracteres.find(a);
  int i_b = caracteres.find(b);

  return table.at(i_b).at(i_a);
}

// clang-format off
char mult_grille(char a, char b, char c, char d, const vector<string> &table,
                 char cle, string &caracteres) {
  switch (cle) {
    case 'F': case 'R': return mult(mult(mult(a, b, table, caracteres), c, table, 
                              caracteres), d, table, caracteres);
    case 'G': case 'S': return mult(mult(mult(a, b, table, caracteres), d, table, 
                              caracteres), c, table, caracteres);
    case 'I': case 'a': return mult(mult(mult(a, c, table, caracteres), b, table, 
                              caracteres), d, table, caracteres);
    case 'K': case 'c': return mult(mult(mult(a, c, table, caracteres), d, table, 
                              caracteres), b,table, caracteres);
    case 'M': case 'q': return mult(mult(mult(a, d, table, caracteres), b, table,
                               caracteres), c, table, caracteres);
    case 'N': case 'r': return mult(mult(mult(a, d, table, caracteres), c, table,
                               caracteres), b, table, caracteres);
    case 'X': case 'd': return mult(mult(mult(b, c, table, caracteres), a, table,
                               caracteres), d, table, caracteres);
    case '[': case 'g': return mult(mult(mult(b, c, table, caracteres), d, table,
                               caracteres), a, table, caracteres);
    case '\\': case 't': return mult(mult(mult(b, d, table, caracteres), a, table,
                               caracteres), c, table, caracteres);
    case '^': case 'v': return mult(mult(mult(b, d, table, caracteres), c, table,
                               caracteres), a, table, caracteres);
    case 'l': case 'x': return mult(mult(mult(c, d, table, caracteres), a, table,
                               caracteres), b, table, caracteres);
    case 'm': case 'y': return mult(mult(mult(c, d, table, caracteres), b, table,
                               caracteres), a, table, caracteres);
    case '0': return mult(mult(a, b, table, caracteres), mult(c, d, table, caracteres),
                          table, caracteres);
    case '1': return mult(mult(a, c, table, caracteres), mult(b, d, table, caracteres),
                          table, caracteres);
    case '2': return mult(mult(a, d, table, caracteres), mult(b, c, table, caracteres),
                          table, caracteres);
    default: print_error(BAD_KEY_VALUE);
  }
  return '0';
}
// clang-format on

vector<string> decryptage(const vector<string> &table,
                          const vector<string> &grille, unsigned int nbL,
                          unsigned int nbC, char cle, string &caracteres) {

  if (((nbL % 2) == 1) || ((nbC % 2) == 1))
    return grille;

  vector<string> resp;
  for (int i(0); i < nbL; i += 2) {
    string ligne;
    for (int j(0); j < nbC; j += 2) {
      ligne.push_back(mult_grille(
          grille.at(i).at(j), grille.at(i).at(j + 1), grille.at(i + 1).at(j),
          grille.at(i + 1).at(j + 1), table, cle, caracteres));
    }
    resp.push_back(ligne);
    cout << ligne << endl;
  }

  cout << endl;

  return decryptage(table, resp, resp.size(), resp.at(0).size(), cle,
                    caracteres);
}