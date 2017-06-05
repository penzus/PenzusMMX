#include <QTextCodec>
#include <bb/cascades/Application>
#include <Qt/qdeclarativedebug.h>

#include "applicationui.h"
#include "applicationui.h"
#define DEBUG_CONSOLE

using namespace bb::cascades;

#ifdef DEBUG_CONSOLE
void myMessageOutput(QtMsgType, const char* msg) {
    fprintf(stdout, "%s\n", msg);
    fflush(stdout);
}
#endif

Q_DECL_EXPORT int main(int argc, char **argv)
{
    ApplicationUI::prepareApplication();
    Application app(argc, argv);

#ifdef DEBUG_CONSOLE
    qInstallMsgHandler(myMessageOutput);
#else
    fclose(stderr);
    fclose(stdout);
#endif

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    new ApplicationUI(&app);

    return Application::exec();
}
