#include "machinecalibration.h"
#include "ui_machinecalibration.h"
#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"

MachineCalibration::MachineCalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MachineCalibration)
{
    ui->setupUi(this);
    load_data();
}

MachineCalibration::~MachineCalibration()
{
    delete ui;
}

void MachineCalibration::load_data(void)
{
 // ui->label_V1_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("V1ToolHomeX"),10));
    ui->label_V1_data->setNum(ExGlobal::getPtr()->getCaliParam("V1ToolHomeX"));
    ui->label_V2_data->setNum(ExGlobal::getPtr()->getCaliParam("V2ToolHomeX"));
    ui->label_V3_data->setNum(ExGlobal::getPtr()->getCaliParam("V3ToolHomeX"));
    ui->label_V4_data->setNum(ExGlobal::getPtr()->getCaliParam("VPToolHomeX"));
    ui->label_door_data->setNum(ExGlobal::getPtr()->getCaliParam("VDWorkX"));
    ui->label_push_data->setNum(ExGlobal::getPtr()->getCaliParam("PumpToolHomeX"));
}


void MachineCalibration::on_pButton_machine_back_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);

}

void MachineCalibration::on_pButton_v1_hard_clicked()
{
    ui->lEdit_V1_data->setText("0");
    Sequence::getPtr()->actionDo("V1",1,0,0,0);
}

void MachineCalibration::on_pButton_v1_soft_clicked()
{
    ui->lEdit_V1_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("V1SoftHomeX"),10));
    Sequence::getPtr()->actionDo("V1",2,0,0,0);

}

void MachineCalibration::on_pButton_v1_work_clicked()
{
    ui->lEdit_V1_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("V1ToolHomeX"),10));
    Sequence::getPtr()->actionDo("V1",9,ui->lEdit_V1_data->text().toInt(),0,0);
}


void MachineCalibration::on_pButton_v1_set_clicked()
{
    Sequence::getPtr()->actionDo("V1",9,ui->lEdit_V1_data->text().toInt(),0,0);

}

void MachineCalibration::on_pButton_v1_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("V1ToolHomeX",ui->lEdit_V1_data->text().toInt());
    ExGlobal::getPtr()->updateCaliParam("V1WorkX",ui->lEdit_V1_data->text().toInt()-ExGlobal::getPtr()->getCaliParam("V1SoftHomeOffset")+ExGlobal::getPtr()->getCaliParam("V1WorkOffset"));
    ExGlobal::getPtr()->updateCaliParam("V1SoftHomeX",ui->lEdit_V1_data->text().toInt()-ExGlobal::getPtr()->getCaliParam("V1SoftHomeOffset"));
    ui->lEdit_V1_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("V1ToolHomeX"),10));
    Sequence::getPtr()->checkParam(true);
}


void MachineCalibration::on_pButton_v2_hard_clicked()
{
    ui->lEdit_V2_data->setText("0");
    Sequence::getPtr()->actionDo("V2",1,0,0,0);
 }

void MachineCalibration::on_pButton_v2_soft_clicked()
{
    ui->lEdit_V2_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("V2SoftHomeX"),10));
    Sequence::getPtr()->actionDo("V2",2,0,0,0);
}

void MachineCalibration::on_pButton_v2_work_clicked()
{
    ui->lEdit_V2_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("V2ToolHomeX"),10));
   Sequence::getPtr()->actionDo("V2",9,ui->lEdit_V2_data->text().toInt(),0,0);
}


void MachineCalibration::on_pButton_v2_set_clicked()
{
    Sequence::getPtr()->actionDo("V2",9,ui->lEdit_V2_data->text().toInt(),0,0);
}

void MachineCalibration::on_pButton_v2_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("V2ToolHomeX",ui->lEdit_V2_data->text().toInt());
    ExGlobal::getPtr()->updateCaliParam("V2WorkX",ui->lEdit_V2_data->text().toInt()-ExGlobal::getPtr()->getCaliParam("V2SoftHomeOffset")+ExGlobal::getPtr()->getCaliParam("V2WorkOffset"));
    ExGlobal::getPtr()->updateCaliParam("V2SoftHomeX",ui->lEdit_V2_data->text().toInt()-ExGlobal::getPtr()->getCaliParam("V2SoftHomeOffset"));
    ui->lEdit_V2_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("V2ToolHomeX"),10));
    Sequence::getPtr()->checkParam(true);
}



void MachineCalibration::on_pButton_v3_hard_clicked()
{
    ui->lEdit_V3_data->setText("0");
    Sequence::getPtr()->actionDo("V3",1,0,0,0);
 }

void MachineCalibration::on_pButton_v3_soft_clicked()
{
    ui->lEdit_V3_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("V3SoftHomeX"),10));
    Sequence::getPtr()->actionDo("V3",2,0,0,0);
}

void MachineCalibration::on_pButton_v3_work_clicked()
{
    ui->lEdit_V3_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("V3ToolHomeX"),10));
   Sequence::getPtr()->actionDo("V3",9,ui->lEdit_V3_data->text().toInt(),0,0);
}


void MachineCalibration::on_pButton_v3_set_clicked()
{
    Sequence::getPtr()->actionDo("V3",9,ui->lEdit_V3_data->text().toInt(),0,0);

}

void MachineCalibration::on_pButton_v3_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("V3ToolHomeX",ui->lEdit_V3_data->text().toInt());
    ExGlobal::getPtr()->updateCaliParam("V3WorkX",ui->lEdit_V3_data->text().toInt()-ExGlobal::getPtr()->getCaliParam("V3SoftHomeOffset")+ExGlobal::getPtr()->getCaliParam("V3WorkOffset"));
    ExGlobal::getPtr()->updateCaliParam("V3SoftHomeX",ui->lEdit_V3_data->text().toInt()-ExGlobal::getPtr()->getCaliParam("V3SoftHomeOffset"));
    ui->lEdit_V3_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("V3ToolHomeX"),10));
    Sequence::getPtr()->checkParam(true);
 }



void MachineCalibration::on_pButton_v4_hard_clicked()
{
    ui->lEdit_V4_data->setText("0");
    Sequence::getPtr()->actionDo("VP",1,0,0,0);
 }

void MachineCalibration::on_pButton_v4_soft_clicked()
{
    ui->lEdit_V4_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("VPSoftHomeX"),10));
    Sequence::getPtr()->actionDo("VP",2,0,0,0);
}

void MachineCalibration::on_pButton_v4_work_clicked()
{
    ui->lEdit_V4_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("VPToolHomeX"),10));
   Sequence::getPtr()->actionDo("VP",9,ui->lEdit_V4_data->text().toInt(),0,0);
}


void MachineCalibration::on_pButton_v4_set_clicked()
{
    Sequence::getPtr()->actionDo("VP",9,ui->lEdit_V4_data->text().toInt(),0,0);
}

void MachineCalibration::on_pButton_v4_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("VPToolHomeX",ui->lEdit_V4_data->text().toInt());
    ExGlobal::getPtr()->updateCaliParam("VPWorkX",ui->lEdit_V4_data->text().toInt()-ExGlobal::getPtr()->getCaliParam("VPSoftHomeOffset")+ExGlobal::getPtr()->getCaliParam("VPWorkOffset"));
    ExGlobal::getPtr()->updateCaliParam("VPSoftHomeX",ui->lEdit_V4_data->text().toInt()-ExGlobal::getPtr()->getCaliParam("VPSoftHomeOffset"));
    ui->lEdit_V4_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("VPToolHomeX"),10));
    Sequence::getPtr()->checkParam(true);
 }


void MachineCalibration::on_pButton_door_hard_clicked()
{
    ui->lEdit_door_data->setText("0");
    Sequence::getPtr()->actionDo("Door",5,0,0,0);
    predoorpos =ui->lEdit_door_data->text().toInt(); //dev5Sp.value;
 }

void MachineCalibration::on_pButton_door_soft_clicked()
{
    ui->lEdit_door_data->setText("0");//QString::number(ExGlobal::getPtr()->getCaliParam("VPSoftHomeX"),10));
    Sequence::getPtr()->actionDo("Door",1,0,0,0);
    predoorpos = ui->lEdit_door_data->text().toInt();
  }

void MachineCalibration::on_pButton_door_work_clicked()
{
    ui->lEdit_door_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("VDWorkX"),10));
    predoorpos = ui->lEdit_door_data->text().toInt();
    Sequence::getPtr()->actionDo("Door",2,0,0,0);
}


void MachineCalibration::on_pButton_door_set_clicked()
{
     if (predoorpos < ui->lEdit_door_data->text().toInt())
        Sequence::getPtr()->actionDo("Door",4,ui->lEdit_door_data->text().toInt()-predoorpos,0,0);
    else
        Sequence::getPtr()->actionDo("Door",3,predoorpos-ui->lEdit_door_data->text().toInt(),0,0);

    predoorpos = ui->lEdit_door_data->text().toInt();


}

void MachineCalibration::on_pButton_door_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("VDWorkX",ui->lEdit_door_data->text().toInt());
    ui->lEdit_door_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("VDWorkX"),10));
    Sequence::getPtr()->checkParam(true);
 }


void MachineCalibration::on_pButton_push_hard_clicked()
{
    ui->lEdit_push_data->setText("0");
    Sequence::getPtr()->actionDo("Pump",1,0,0,0);
 }

void MachineCalibration::on_pButton_push_soft_clicked()
{
    ui->lEdit_push_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("PumpSoftHomeX"),10));
    Sequence::getPtr()->actionDo("Pump",9,ui->lEdit_push_data->text().toInt(),0,0);
    predoorpos = ui->lEdit_door_data->text().toInt();
}

void MachineCalibration::on_pButton_push_work_clicked()
{
    ui->lEdit_push_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("PumpToolHomeX"),10));
    Sequence::getPtr()->actionDo("Pump",9,ui->lEdit_push_data->text().toInt(),0,0);
}


void MachineCalibration::on_pButton_push_set_clicked()
{
    Sequence::getPtr()->actionDo("Pump",9,ui->lEdit_push_data->text().toInt(),0,0);
}

void MachineCalibration::on_pButton_push_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("PumpToolHomeX",ui->lEdit_push_data->text().toInt());
    ExGlobal::getPtr()->updateCaliParam("PumpSoftHomeX",ui->lEdit_push_data->text().toInt()-ExGlobal::getPtr()->getCaliParam("PumpSoftHomeOffset"));
    ui->lEdit_push_data->setText(QString::number(ExGlobal::getPtr()->getCaliParam("PumpToolHomeX"),10));
    Sequence::getPtr()->checkParam(true);
}


void MachineCalibration::on_pButton_door_in_clicked()
{
    Sequence::getPtr()->actionDo("Door",4,ui->lEdit_door_step->text().toInt(),ui->lEdit_gear->text().toInt(),0);
}

void MachineCalibration::on_pButton_door_out_clicked()
{
    Sequence::getPtr()->actionDo("Door",3,ui->lEdit_door_step->text().toInt(),ui->lEdit_gear->text().toInt(),0);
}
