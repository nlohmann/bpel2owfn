#ifndef GUI_ORGANIZER_H
#define GUI_ORGANIZER_H

#include <QObject>
#include <string>

using std::string;


class GUI_Organizer : public QObject
{
  Q_OBJECT
  
  private:
    int reduction_level;
  
    bool use_long_options;
    
    bool fileFormat_fiona;
    bool fileFormat_lola;
    bool fileFormat_pnml;
    bool fileFormat_dot;
    
    string output_filename;
    string command_line;
    
    int patterns;
    bool patterns_variables;
  
  public:
    GUI_Organizer();
  
  public slots:
    void set_commandLine();
    void set_reductionLevel(int value);
    void set_longOptions(bool value);
    void set_fileFormatFiona(bool value);
    void set_fileFormatLola(bool value);
    void set_fileFormatPnml(bool value);
    void set_fileFormatDot(bool value);
    void set_outputFileName(QString value);
    void set_patternSmall(bool value);
    void set_patternNofhfaults(bool value);
    void set_patternStandardfaults(bool value);
    void set_patternVariables(bool value);
  
  signals:
    void reductionLevel_changed(QString text);
    void commandLine_changed(QString text);
    void no_variables_possible(bool value);
};

#endif