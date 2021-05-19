#include "Oboard.hh"

const int order[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

bool Board::isFilled(int x, int y) {
  return (filled[y] & (0b10000000 >> x));
};

bool Board::isWhite(int x, int y) {
  return (coloredWhite[y] & (0b10000000 >> x));
};

void Board::flipToWhite(int x, int y) {
  filled[y] = filled[y] | (0b10000000 >> x);
  coloredWhite[y] = coloredWhite[y] | (0b10000000 >> x);
};

void Board::flipToBlack(int x, int y) {
  filled[y] = filled[y] | (0b10000000 >> x);
  coloredWhite[y] = coloredWhite[y] & (~(0b10000000 >> x));
};

Board::Board() {
  for (int i = 0; i < 8; i++) {
    if (i < 5 && i > 2) {
      filled[i] = 0b00011000;
      coloredWhite[i] = (i==3)? 0b00001000:0b00010000;
    } else {
      filled[i] = 0;
      coloredWhite[i] = 0;
    }
  }
  turnAndTile = 0b10000100; //white's turn and 4 pieces on Board
  scoreInt = 0;
}

Board::Board(Board* b) {
  this->copy(b);
}

int Board::score () { // # of white tiles - # black tiles
  return this->scoreInt;
}

int Board::value() {
  int value = this->scoreInt;

  //maybe add linear change to value of score vs terriory?

  const int cornerVal = 8; //how much more valuable is a corner than any other flip
  if (isFilled(0,0)) {
    value += (isWhite(0,0))? cornerVal:-1*cornerVal;
  }
  if (isFilled(0,7)) {
    value += (isWhite(0,7))? cornerVal:-1*cornerVal;
  }
  if (isFilled(7,0)) {
    value += (isWhite(7,0))? cornerVal:-1*cornerVal;
  }
  if (isFilled(7,7)) {
    value += (isWhite(7,7))? cornerVal:-1*cornerVal;
  }
  return value;
}

int Board::tileNum () {
  return (int)(turnAndTile & 0b01111111); //last seven bits
}

Board* Board::move(bool playWhite, int x, int y) {
  if (x > 7 || x < 0 || y > 7 || y < 0) return nullptr;
  if (isFilled(x,y)) return nullptr;
  int tmpx, tmpy, distance, end;
  int toFlip[8];
  for (int ray = 0; ray < 8; ray++) { //iter over cardinal + diagonals
    distance = 1;
    end = 0;
    while (end == 0) {
      tmpx = x + (distance*order[ray][0]);
      tmpy = y + (distance*order[ray][1]);

      if (tmpx < 0 || tmpx > 7 || tmpy < 0 || tmpy > 7) end = 1; //ran off edge
      else if (!isFilled(tmpx,tmpy)) end = 2; //ran into an empty space
      else if (isWhite(tmpx,tmpy) == playWhite) end = (distance > 1)? 3:4; //ran into own color late vs early
      else distance++;
    }
    if (end == 3) toFlip[ray] = distance;
    else toFlip[ray] = 0;
  } //checked all rays and populated toFlip

  bool legal = false;
  for (int i : toFlip) { //any legal flips from playing here?
    if (i != 0) {
      legal = true;
      break;
    }
  }

  if (legal) {
    int numFlipped = 0;
    Board* c = new Board(this);

    for (int r = 0; r < 8; r++) { //rays, must be at least 1 that is > 0
      if (toFlip[r] != 0) numFlipped += toFlip[r]-1;
      for (int d = 1; d < toFlip[r]; d++) { //above calced length
        if (playWhite) c->flipToWhite(x + (d*order[r][0]), y + (d*order[r][1])); //update Board c according to color
        else c->flipToBlack(x + (d*order[r][0]), y + (d*order[r][1]));
      }
    }
    if (playWhite) c->flipToWhite(x,y); //place new tile
    else c->flipToBlack(x,y);

    numFlipped = (2*numFlipped)+1; //account for placed tile and that score -1 for lost white and -1 from new black of each flip. thus 2*flip + 1
    char tn = (this->turnAndTile & 0b01111111) + 1;
    if (!playWhite) {
      tn = tn | 0b10000000; //change turn back. if just played white, then its B's turn and it can stay as a 0;
      c->scoreInt = this->scoreInt - numFlipped; //play black, score decreases
    } else {
      c->scoreInt = this->scoreInt + numFlipped; //play white, score increases
    }
    c->turnAndTile = tn;
    return c;
  } else { //not legal
    return nullptr;
  }
}

void Board::copy(Board* from) {
  for (int i = 0; i < 8; i++) {
    filled[i] = from->filled[i];
    coloredWhite[i] = from->coloredWhite[i];
  }
  turnAndTile = from->turnAndTile;
  scoreInt = from->scoreInt;
}

std::string Board::toString(bool big) {
  if (big) return toStringBig();
  else return toStringSmall();
}

std::string Board::toString() {
  return toString(false);
}

std::string Board::toStringSmall() {
  std::string out = " 01234567\n";
  const char esc = 27;
  std::string fg, bg, tile;
  for (int y = 0; y < 8; y++) {
    out += std::to_string(y);
    for (int x = 0; x < 8; x++) {
      bg = (y%2 == x%2)? "42":"43";
      fg = (isWhite(x,y))? "37":"30";
      tile = (isWhite(x,y))? "W":"B";
      if (!isFilled(x,y)) tile = " ";
      //out += esc + "[" + fg + ";" + bg + "m" + tile;
      out.push_back(esc);
      out.push_back('[');
      out += fg;
      out.push_back(';');
      out += bg;
      out.push_back('m');
      out += tile;
    }
    out.push_back(esc);
    out += "[0m";
    out += std::to_string(y);
    out += '\n';
  }
  out += " 01234567\n";
  return out;
}

std::string Board::toStringBig() {
  std::string out = "";
  const char esc = 27;
  std::string fg,bg,tile;
  std::string reset = "";
  reset.push_back(esc);
  reset.push_back('[');
  reset += "0m";

  out.push_back(esc);
  out += "[1;1H";
  out.push_back(esc);
  out += "[0J";
  out += "  0  1  2  3  4  5  6  7 \n";

  for (int y = 0; y < 8; y++) {
    out += " ";
    for (int c = 0; c < 8; c++) {
      bg = (c%2 == y%2)? "42":"43";
      out.push_back(esc);
      out.push_back('[');
      out += ";" + bg; //no fg
      out.push_back('m');
      out += "   ";
    }  
    out += reset + "\n";
    out += std::to_string(y);
    for (int x = 0; x < 8; x++) {
      bg = (y%2 == x%2)? "42":"43";
      fg = (isWhite(x,y))? "37":"30";
      tile = " "; // consider revising
      if (isFilled(x,y)){
        tile = (isWhite(x,y))? "W":"B"; 
      } else tile = " ";
      out.push_back(esc);
      out.push_back('[');
      out += fg;
      out.push_back(';');
      out += bg;
      out.push_back('m');
      out += " " + tile + " ";
    }
    out += reset + std::to_string(y) + "\n";
    out += " ";
    for (int c = 0; c < 8; c++) {
      bg = (c%2 == y%2)? "42":"43";
      out.push_back(esc);
      out.push_back('[');
      out += ";" + bg; //no fg
      out.push_back('m');
      out += "   ";
    }  
    out += reset + "\n";
  }
  out += "  0  1  2  3  4  5  6  7 \n";
  return out;
}

std::deque<Board*> Board::children (bool playingWhite) {
  std::deque<Board*> out = std::deque<Board*>();
  Board* d;
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if ((d = move(playingWhite,x,y)) != nullptr) {
        out.push_front(d);
      }
    }
  }
  return out;
}

bool Board::whitesTurn() {
  return ((turnAndTile & 0b10000000) != 0);
}

bool Board::isLegal(bool playWhite, int x, int y) { //slightly modified version of move()
  if (x > 7 || x < 0 || y > 7 || y < 0) return false;
  if (isFilled(x,y)) return false;
  int tmpx, tmpy, distance, end;
  for (int ray = 0; ray < 8; ray++) { //iter over cardinal + diagonals
    distance = 1;
    end = 0;
    while (end == 0) {
      tmpx = x + (distance*order[ray][0]);
      tmpy = y + (distance*order[ray][1]);

      if (tmpx < 0 || tmpx > 7 || tmpy < 0 || tmpy > 7) end = 1; //ran off edge
      else if (!isFilled(tmpx,tmpy)) end = 2; //ran into an empty space
      else if (isWhite(tmpx,tmpy) == playWhite) end = (distance > 1)? 3:4; //ran into own color late vs early
      else distance++;
    }
    if (end == 3) return true;
  } 

  return false;
}

bool Board::anyLegalMoves(bool playWhite) {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (isLegal(playWhite,x,y)) return true;
    }
  }
  return false;
}

bool Board::operator==(const Board& b) {
  if (turnAndTile != b.turnAndTile) return false;
  for (int i : {3,4,2,5,1,6,0,7}) {
    if (!(filled[i] == b.filled[i] && coloredWhite[i] == b.coloredWhite[i])) return false;
  }
  return true;
}

