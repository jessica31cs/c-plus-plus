// lexical analyzer header
// cs280
// problem 2
// fall 2016

#include <string>
#include <iostream>

enum TokenType {
	ID,		// identifier
	STR,		// string
	INT,		// integer constant
	PLUS,		// the + operator
	STAR,		// the * operator
	LEFTSQ,		// the [
	RIGHTSQ,	// the ]
	PRINT,		// print keyword
	SET,		// set keyword
	SC,		// semicolon
	LPAREN,		// The (
	RPAREN,		// the )
	DONE,		// finished!
	ERR,		// an unrecoverable error
};

class Token {
private:
	TokenType	tok;
	std::string	lexeme;

public:
	Token() : tok(ERR), lexeme("") {}
	Token(TokenType t, std::string s) : tok(t), lexeme(s) {}

	TokenType	getTok() const		{ return tok; }
	std::string	getLexeme() const	{ return lexeme; }

	friend bool operator==(const Token& left, const Token& right) {
		return left.tok == right.tok;
	}

	friend std::ostream& operator<<(std::ostream& out, const Token& t);
};

extern	int	linenum;
extern	Token	getToken(std::istream* instream);
