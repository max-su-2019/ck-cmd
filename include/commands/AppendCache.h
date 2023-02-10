#include "stdafx.h"

// Command Base
#include <commands/CommandBase.h>

class AppendCacheCMD : public Command<AppendCacheCMD>
{
	REGISTER_COMMAND_HEADER(AppendCacheCMD)

private:
	AppendCacheCMD();
	virtual ~AppendCacheCMD();

public:
	virtual string GetName() const;
	virtual string GetHelp() const;
	virtual string GetHelpShort() const;

protected:
	virtual bool InternalRunCommand(map<string, docopt::value> parsedArgs);
};