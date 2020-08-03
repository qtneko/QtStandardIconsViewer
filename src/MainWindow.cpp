#include "MainWindow.h"
#include <QApplication>
#include <QClipboard>
#include <QMetaEnum>
#include <QMenu>
#include <QPushButton>
#include <QScrollBar>
#include <QStyle>
#include <QSystemTrayIcon>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QScrollArea(parent)
{
    _style = QApplication::style();

    setWindowTitle(tr("Standard Icons Viewer"));
    setWindowIcon(_style->standardIcon(QStyle::SP_TitleBarMenuButton));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setupWidgets();

    setFixedWidth(sizeHint().width() + verticalScrollBar()->sizeHint().width());
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupWidgets()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QMetaEnum metaEnum = QMetaEnum::fromType<QStyle::StandardPixmap>();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    const auto lastValueInEnum = QStyle::SP_RestoreDefaultsButton;
#elif (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
    const auto lastValueInEnum = QStyle::SP_LineEditClearButton;
#else
    const auto lastValueInEnum = QStyle::SP_MediaVolumeMuted;
#endif
    for (int i = 0; i <= static_cast<int>(lastValueInEnum); ++i)
    {
        const auto pix = static_cast<QStyle::StandardPixmap>(i);
        const auto name = "QStyle::" + QString::fromStdString(std::string(metaEnum.valueToKey(pix)));
        auto icon = _style->standardIcon(pix);
        QPushButton *btn = new QPushButton(icon, name);
        btn->setIconSize(QSize(32, 32));
        btn->setStyleSheet("text-align:left;");
        mainLayout->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::processBtnClick);
    }
    mainLayout->addStretch();
    QWidget *widget = new QWidget;
    widget->setLayout(mainLayout);
    setWidget(widget);

    setupTrayWidget();
}

void MainWindow::setupTrayWidget()
{
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        _trayWidget = new QSystemTrayIcon(windowIcon(), this);
        _trayWidget->setToolTip(windowTitle());
        _trayWidget->setVisible(true);

        QMenu *menu = new QMenu(this);
        auto *quitAction = menu->addAction(_style->standardIcon(QStyle::SP_TitleBarCloseButton), tr("Quit"), this, &MainWindow::close);
        Q_UNUSED(quitAction);
        _trayWidget->setContextMenu(menu);
    }
}

void MainWindow::processBtnClick()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
    {
        return;
    }
    const QString name = btn->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(name);
    if (_trayWidget)
    {
        _trayWidget->showMessage(windowTitle(), tr("'%1' is copied to clipboard").arg(name), btn->icon());
    }
}
