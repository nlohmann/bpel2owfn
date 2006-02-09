#ifndef OPTIONS_H
#define OPTIONS_H

#include <map>
#include "getopt.h"

typedef enum
{
   O_HELP,
   O_VERSION,
   O_INPUT,
   O_OUTPUT,
   O_MODE,
   O_LOG,
   O_FORMAT,
   O_PARAMETER,
   O_DEBUG
} possibleOptions;

typedef enum
{
  M_AST = 1,
  M_PRETTY,
  M_PETRINET,
  M_CFG
} possibleModi;

typedef enum
{
  F_LOLA,
  F_OWFN,
  F_DOT,
  F_PEP,
  F_APPN,
  F_INFO,
  F_PNML,
  F_TXT,
  F_XML
} possibleFormats;

typedef enum
{
  P_SIMPLIFY,
  P_NOINTERFACE,
  P_FINALMARKING,
  P_UNIQUEFAULT,
  P_FINISHLOOP,
  P_NOSTANDARDFAULTS
} possibleParameters;


// some file names and pointers (in options.cc)

/// Filename of input file
extern std::string filename;
/// Filename of input file
extern std::string output_filename;
/// Filename of log file
extern std::string log_filename;

/// pointer to input stream
extern std::istream * input;
/// pointer to output stream
extern std::ostream * output;
/// pointer to log stream
extern std::ostream * log_output;

extern bool createOutputFile;

// different modes controlled by command line (in options.cc)

extern possibleModi modus;

extern std::map<possibleOptions,    bool> options;
extern std::map<possibleParameters, bool> parameters;
extern std::map<possibleFormats,    bool> formats;
// suffixes are defined in parse_command_line();
extern std::map<possibleFormats,  std::string> suffixes;


/* defined by Bison */
extern int yydebug;

/* defined by flex */
extern int yy_flex_debug;
extern FILE *yyin;

// returns an open file pointer
extern std::ostream * openOutput(std::string name);
extern void closeOutput(std::ostream * file);

#endif

