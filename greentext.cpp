#include <znc/main.h>
#include <znc/Modules.h>

class Greentext : public CModule
{
private:
    void ApplyGreentext(CString &message) {
        if (message.Left(1) == ">")
            message = "\00303" + message + "\003";
    }

    void ShareCommand(const CString &line) {
        SetNV("share", "");
        PutModule("Sharing enabled");
    }

    void UnshareCommand(const CString &line) {
        DelNV("share");
        PutModule("Sharing disabled");
    }

public:
    MODCONSTRUCTOR(Greentext) {
        AddHelpCommand();
        AddCommand("Share", static_cast<CModCommand::ModCmdFunc>(&Greentext::ShareCommand),
                   "", "Enable greentext sharing");
        AddCommand("Unshare", static_cast<CModCommand::ModCmdFunc>(&Greentext::UnshareCommand),
                   "", "Disable greentext sharing");
    }

    virtual EModRet OnChanMsg(CNick &nick, CChan &channel, CString &message) {
        ApplyGreentext(message);
        return CONTINUE;
    }

    virtual EModRet OnPrivMsg(CNick &nick, CString &message) {
        ApplyGreentext(message);
        return CONTINUE;
    }

    virtual EModRet OnUserMsg(CString &target, CString &message) {
        if (FindNV("share") != EndNV())
            ApplyGreentext(message);
        return CONTINUE;
    }
};

MODULEDEFS(Greentext, "Enables >greentext")
