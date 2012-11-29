#ifndef DIALOG_SPEC_HELPER_HPP
#define DIALOG_SPEC_HELPER_HPP

class DialogSpecHelper : public QObject
{
    Q_OBJECT

    QTimer code_inject_timer;
    bool injected_method_succeded_flag;
    bool accept;
public:
    DialogSpecHelper(bool _accept = false) :
        injected_method_succeded_flag(false),
        accept(_accept)
    {
        code_inject_timer.setSingleShot(true);
        code_inject_timer.setInterval(1);
        code_inject_timer.start();
        connect(&code_inject_timer, SIGNAL(timeout()), SLOT(injection_time()));
    }

    ~DialogSpecHelper()
    {
        // make sure timer doesn't fire anymore
        if (code_inject_timer.isActive())
            code_inject_timer.stop();
    }

    bool injected_method_ran() const
    {
        return ! code_inject_timer.isActive();
    }

    bool injected_method_succeded() const
    {
        return injected_method_succeded_flag;
    }

signals:
    void run_injected_code(QWidget * active_modal);

protected slots:
    void injection_time()
    {
        QWidget * active_modal = qApp->activeModalWidget();
        Finalizer finalizer(active_modal, accept);
        emit run_injected_code(active_modal);
        injected_method_succeded_flag = true;
    }
protected:
    class Finalizer {
        QDialog * dialog;
        bool accept;
    public:
        Finalizer( QWidget * active_modal, bool _accept) :
            dialog( qobject_cast<QDialog*>(active_modal) ),
            accept(_accept)
        {}

        ~Finalizer() {
            if (dialog) {
                if (accept)
                    dialog->accept();
                else
                    dialog->reject();
            }
        }
    };
};

#endif // DIALOG_SPEC_HELPER_HPP
