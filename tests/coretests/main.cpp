#include "tests.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>

#include <QtTest/QTest>

int main(int argc, char *argv[])
{
    QCoreApplication coreTestsApp(argc, argv);
    coreTestsApp.setApplicationName("CoreTests");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", ""));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
        { { "o", "ommit" },
          QCoreApplication::translate("main", "Ommits test"),
          QCoreApplication::translate("main", "Test to ommit") }
    });
    parser.process(coreTestsApp);

    ExtractorTest extractor;
    QTest::qExec(&extractor);

    AccountInfoExtractorTest accountInfo;
    QTest::qExec(&accountInfo);

    return 0;
}
