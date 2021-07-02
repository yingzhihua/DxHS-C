#include "pidparam.h"
#include "ui_pidparam.h"

#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"


PIDParam::PIDParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PIDParam)
{
    ui->setupUi(this);
    show_data();
}

PIDParam::~PIDParam()
{
    delete ui;
}

void PIDParam::show_data(void)
{
    //ui->label_PID1_P_TXT.
    ui->label_PID1_P_TXT->setNum(ExGlobal::getCaliParam("PID_P1"));
    ui->label_PID1_I_TXT->setNum(ExGlobal::getCaliParam("PID_I1"));
     ui->label_PID1_D_TXT->setNum(ExGlobal::getCaliParam("PID_D1"));

     ui->label_PID2_P_TXT->setNum(ExGlobal::getCaliParam("PID_P2"));
     ui->label_PID2_I_TXT->setNum(ExGlobal::getCaliParam("PID_I2"));
      ui->label_PID2_D_TXT->setNum(ExGlobal::getCaliParam("PID_D2"));

      ui->label_PID3_P_TXT->setNum(ExGlobal::getCaliParam("PID_P3"));
      ui->label_PID3_I_TXT->setNum(ExGlobal::getCaliParam("PID_I3"));
       ui->label_PID3_D_TXT->setNum(ExGlobal::getCaliParam("PID_D3"));

     ui->label_PID4_P_TXT->setNum(ExGlobal::getCaliParam("PID_P4"));
     ui->label_PID4_I_TXT->setNum(ExGlobal::getCaliParam("PID_I4"));
     ui->label_PID4_D_TXT->setNum(ExGlobal::getCaliParam("PID_D4"));

     ui->label_PID5_P_TXT->setNum(ExGlobal::getCaliParam("PID_P5"));
     ui->label_PID5_I_TXT->setNum(ExGlobal::getCaliParam("PID_I5"));
     ui->label_PID5_D_TXT->setNum(ExGlobal::getCaliParam("PID_D5"));

     ui->label_PID6_P_TXT->setNum(ExGlobal::getCaliParam("PID_P6"));
     ui->label_PID6_I_TXT->setNum(ExGlobal::getCaliParam("PID_I6"));
      ui->label_PID6_D_TXT->setNum(ExGlobal::getCaliParam("PID_D6"));

      ui->label_PID7_P_TXT->setNum(ExGlobal::getCaliParam("PID_P7"));
      ui->label_PID7_I_TXT->setNum(ExGlobal::getCaliParam("PID_I7"));
       ui->label_PID7_D_TXT->setNum(ExGlobal::getCaliParam("PID_D7"));

       ui->label_PID8_P_TXT->setNum(ExGlobal::getCaliParam("PID_P8"));
       ui->label_PID8_I_TXT->setNum(ExGlobal::getCaliParam("PID_I8"));
        ui->label_PID8_D_TXT->setNum(ExGlobal::getCaliParam("PID_D8"));

      ui->label_PID9_P_TXT->setNum(ExGlobal::getCaliParam("PID_P9"));
      ui->label_PID9_I_TXT->setNum(ExGlobal::getCaliParam("PID_I9"));
      ui->label_PID9_D_TXT->setNum(ExGlobal::getCaliParam("PID_D9"));

      ui->label_PID10_P_TXT->setNum(ExGlobal::getCaliParam("PID_P10"));
      ui->label_PID10_I_TXT->setNum(ExGlobal::getCaliParam("PID_I10"));
      ui->label_PID10_D_TXT->setNum(ExGlobal::getCaliParam("PID_D10"));


      ui->label_PID11_P_TXT->setNum(ExGlobal::getCaliParam("PID_P11"));
      ui->label_PID11_I_TXT->setNum(ExGlobal::getCaliParam("PID_I11"));
       ui->label_PID11_D_TXT->setNum(ExGlobal::getCaliParam("PID_D11"));

       ui->label_PID12_P_TXT->setNum(ExGlobal::getCaliParam("PID_P12"));
       ui->label_PID12_I_TXT->setNum(ExGlobal::getCaliParam("PID_I12"));
        ui->label_PID12_D_TXT->setNum(ExGlobal::getCaliParam("PID_D12"));

        ui->label_PID13_P_TXT->setNum(ExGlobal::getCaliParam("PID_P13"));
        ui->label_PID13_I_TXT->setNum(ExGlobal::getCaliParam("PID_I13"));
         ui->label_PID13_D_TXT->setNum(ExGlobal::getCaliParam("PID_D13"));

       ui->label_PID14_P_TXT->setNum(ExGlobal::getCaliParam("PID_P14"));
       ui->label_PID14_I_TXT->setNum(ExGlobal::getCaliParam("PID_I14"));
       ui->label_PID14_D_TXT->setNum(ExGlobal::getCaliParam("PID_D14"));

       ui->label_PID15_P_TXT->setNum(ExGlobal::getCaliParam("PID_P15"));
       ui->label_PID15_I_TXT->setNum(ExGlobal::getCaliParam("PID_I15"));
       ui->label_PID15_D_TXT->setNum(ExGlobal::getCaliParam("PID_D15"));


}

void PIDParam::on_PIDParam_accepted()
{

}

void PIDParam::on_pButton_machine_back_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
