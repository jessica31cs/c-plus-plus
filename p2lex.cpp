#include <cctype>
#include <map>
#include "p2lex.h"
using namespace std;

map<TokenType,string> TokenNames = {
	{ ID, "id" },
	{ STR, "str" },
	{ INT, "int" },
	{ PLUS, "plus" },
	{ STAR, "star" },
	{ LEFTSQ, "leftsq" },
	{ RIGHTSQ, "rightsq" },
	{ PRINT, "print" },
	{ SET, "set" },
	{ SC, "sc" },
	{ LPAREN, "lparen" },
	{ RPAREN, "rparen" },
	{ DONE, "done" },
	{ ERR, "err" },
};

string getPrintName(TokenType t)
{
	return TokenNames[t];
}

map<TokenType,bool> TokenExtras = {
	{ ID, true },
	{ STR, true },
	{ INT, true },
	{ ERR, true },
};

ostream& operator<<(ostream& out, const Token& t)
{
	out << TokenNames[t.tok];
	if( TokenExtras[t.tok] ) {
		out << "(" << t.lexeme << ")";
	}
	return out;
}

Token getToken(std::istream* instream)
{
	enum LexState { BEGIN, INID, INSTR, ININT, INCOMMENT } state = BEGIN;

	int	ch;
	string	lexeme;

	while( (ch = instream->get()) != EOF ) {
		switch( state ) {
		case BEGIN:
			if( ch == '\n' )
				++linenum;

			if( isspace(ch) )
				continue;

            if( ch == '"' ) {
				state = INSTR;
				continue;
			}

			lexeme = ch;

			if( isalpha(ch) ) {
				state = INID;
				continue;
			}

			if( isdigit(ch) ) {
				state = ININT;
				continue;
			}

			if( ch == '/' && instream->peek() == '/' ) {
				state = INCOMMENT;
				continue;
			}

			switch( ch ) {
				case '+':
					return Token(PLUS, lexeme);

				case '*':
					return Token(STAR, lexeme);

				case '[':
					return Token(LEFTSQ, lexeme);

				case ']':
					return Token(RIGHTSQ, lexeme);

				case '(':
					return Token(LPAREN, lexeme);

				case ')':
					return Token(RPAREN, lexeme);

				case ';':
					return Token(SC, lexeme);
			}

			return Token(ERR, lexeme);

			break;

		case INID:
			if( isalpha(ch) ) {
				lexeme += ch;
				continue;
			}

			instream->putback(ch);
			if( lexeme == "print" )
				return Token(PRINT, lexeme);
			if( lexeme == "set" )
				return Token(SET, lexeme);
			return Token(ID, lexeme);

		case INSTR:
			if( ch == '\n' )
				return Token(ERR, lexeme);

			lexeme += ch;

			if( ch != '"' ) {
				continue;
			}

            lexeme = lexeme.substr(0,lexeme.length()-1);
			return Token(STR, lexeme);

		case ININT:
			if( isdigit(ch) ) {
				lexeme += ch;
				continue;
			}

			instream->putback(ch);
			return Token(INT, lexeme);

		case INCOMMENT:
			if( ch != '\n' )
				continue;

			state = BEGIN;
			break;

		default:
			cerr << "Value of state is not known!" << endl;
			return Token(); // error
		}
	}

	if( state == BEGIN )
		return Token(DONE, "");
	else if( state == INID )
		return Token(ID, lexeme);
	else if( state == ININT )
		return Token(INT, lexeme);
	else
		return Token(ERR, lexeme);
}
