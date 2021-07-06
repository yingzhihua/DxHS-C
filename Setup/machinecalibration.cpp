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
    ui->pButton_machine_back->setGeometry(1580,740,300,115);

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
    ui->spinBox_V1->setValue(0);

    Sequence::getPtr()->actionDo("V1",1,0,0,0);
}

void MachineCalibration::on_pButton_v1_soft_clicked()
{
    ui->spinBox_V1->setValue(ExGlobal::getPtr()->getCaliParam("V1SoftHomeX"));
    Sequence::getPtr()->actionDo("V1",2,0,0,0);
}

void MachineCalibration::on_pButton_v1_work_clicked()
{
     ui->spinBox_V1->setValue(ExGlobal::getPtr()->getCaliParam("V1ToolHomeX"));
    Sequence::getPtr()->actionDo("V1",9,ui->spinBox_V1->value(),0,0);
}


void MachineCalibration::on_pButton_v1_set_clicked()
{
    Sequence::getPtr()->actionDo("V1",9,ui->spinBox_V1->value(),0,0);

}

void MachineCalibration::on_pButton_v1_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("V1ToolHomeX",ui->spinBox_V1->value());
    ExGlobal::getPtr()->updateCaliParam("V1WorkX",ui->spinBox_V1->value()-ExGlobal::getPtr()->getCaliParam("V1SoftHomeOffset")+ExGlobal::getPtr()->getCaliParam("V1WorkOffset"));
    ExGlobal::getPtr()->updateCaliParam("V1SoftHomeX",ui->spinBox_V1->value()-ExGlobal::getPtr()->getCaliParam("V1SoftHomeOffset"));
    ui->spinBox_V1->setValue(ExGlobal::getPtr()->getCaliParam("V1ToolHomeX"));
    Sequence::getPtr()->checkParam(true);
}


void MachineCalibration::on_pButton_v2_hard_clicked()
{
    ui->spinBox_V2->setValue(0);
    Sequence::getPtr()->actionDo("V2",1,0,0,0);
 }

void MachineCalibration::on_pButton_v2_soft_clicked()
{
    ui->spinBox_V2->setValue(ExGlobal::getPtr()->getCaliParam("V2SoftHomeX"));
    Sequence::getPtr()->actionDo("V2",2,0,0,0);
}

void MachineCalibration::on_pButton_v2_work_clicked()
{
    ui->spinBox_V2->setValue(ExGlobal::getPtr()->getCaliParam("V2ToolHomeX"));
   Sequence::getPtr()->actionDo("V2",9,ui->spinBox_V2->value(),0,0);
}


void MachineCalibration::on_pButton_v2_set_clicked()
{
    Sequence::getPtr()->actionDo("V2",9,ui->spinBox_V2->value(),0,0);
}

void MachineCalibration::on_pButton_v2_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("V2ToolHomeX",ui->spinBox_V2->value());
    ExGlobal::getPtr()->updateCaliParam("V2WorkX",ui->spinBox_V2->value()-ExGlobal::getPtr()->getCaliParam("V2SoftHomeOffset")+ExGlobal::getPtr()->getCaliParam("V2WorkOffset"));
    ExGlobal::getPtr()->updateCaliParam("V2SoftHomeX",ui->spinBox_V2->value()-ExGlobal::getPtr()->getCaliParam("V2SoftHomeOffset"));
    ui->spinBox_V2->setValue(ExGlobal::getPtr()->getCaliParam("V2ToolHomeX"));
    Sequence::getPtr()->checkParam(true);
}



void MachineCalibration::on_pButton_v3_hard_clicked()
{
    ui->spinBox_V3->setValue(0);
    Sequence::getPtr()->actionDo("V3",1,0,0,0);
 }

void MachineCalibration::on_pButton_v3_soft_clicked()
{
    ui->spinBox_V3->setValue(ExGlobal::getPtr()->getCaliParam("V3SoftHomeX"));
    Sequence::getPtr()->actionDo("V3",2,0,0,0);
}

void MachineCalibration::on_pButton_v3_work_clicked()
{
   ui->spinBox_V3->setValue(ExGlobal::getPtr()->getCaliParam("V3ToolHomeX"));
   Sequence::getPtr()->actionDo("V3",9,ui->spinBox_V3->value(),0,0);
}


void MachineCalibration::on_pButton_v3_set_clicked()
{
    Sequence::getPtr()->actionDo("V3",9,ui->spinBox_V3->value(),0,0);

}

void MachineCalibration::on_pButton_v3_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("V3ToolHomeX",ui->spinBox_V3->value());
    ExGlobal::getPtr()->updateCaliParam("V3WorkX",ui->spinBox_V3->value()-ExGlobal::getPtr()->getCaliParam("V3SoftHomeOffset")+ExGlobal::getPtr()->getCaliParam("V3WorkOffset"));
    ExGlobal::getPtr()->updateCaliParam("V3SoftHomeX",ui->spinBox_V3->value()-ExGlobal::getPtr()->getCaliParam("V3SoftHomeOffset"));
    ui->spinBox_V3->setValue(ExGlobal::getPtr()->getCaliParam("V3ToolHomeX"));
    Sequence::getPtr()->checkParam(true);
 }



void MachineCalibration::on_pButton_v4_hard_clicked()
{
    ui->spinBox_V4->setValue(0);
    Sequence::getPtr()->actionDo("VP",1,0,0,0);
 }

void MachineCalibration::on_pButton_v4_soft_clicked()
{
    ui->spinBox_V4->setValue(ExGlobal::getPtr()->getCaliParam("VPSoftHomeX"));
    Sequence::getPtr()->actionDo("VP",2,0,0,0);
}

void MachineCalibration::on_pButton_v4_work_clicked()
{
   ui->spinBox_V4->setValue(ExGlobal::getPtr()->getCaliParam("VPToolHomeX"));
   Sequence::getPtr()->actionDo("VP",9,ui->spinBox_V4->value(),0,0);
}


void MachineCalibration::on_pButton_v4_set_clicked()
{
    Sequence::getPtr()->actionDo("VP",9,ui->spinBox_V4->value(),0,0);
}

void MachineCalibration::on_pButton_v4_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("VPToolHomeX",ui->spinBox_V4->value());
    ExGlobal::getPtr()->updateCaliParam("VPWorkX",ui->spinBox_V4->value()-ExGlobal::getPtr()->getCaliParam("VPSoftHomeOffset")+ExGlobal::getPtr()->getCaliParam("VPWorkOffset"));
    ExGlobal::getPtr()->updateCaliParam("VPSoftHomeX",ui->spinBox_V4->value()-ExGlobal::getPtr()->getCaliParam("VPSoftHomeOffset"));
    ui->spinBox_V4->setValue(ExGlobal::getPtr()->getCaliParam("VPToolHomeX"));
    Sequence::getPtr()->checkParam(true);
 }


void MachineCalibration::on_pButton_door_hard_clicked()
{
    ui->spinBox_Door->setValue(0);
    Sequence::getPtr()->actionDo("Door",5,0,0,0);
    predoorpos =ui->spinBox_Door->value();
 }

void MachineCalibration::on_pButton_door_soft_clicked()
{
    ui->spinBox_Door->setValue(0);
    Sequence::getPtr()->actionDo("Door",1,0,0,0);
    predoorpos = ui->spinBox_Door->value();
  }

void MachineCalibration::on_pButton_door_work_clicked()
{
    ui->spinBox_Door->setValue(ExGlobal::getPtr()->getCaliParam("VDWorkX"));
    predoorpos = ui->spinBox_Door->value();
    Sequence::getPtr()->actionDo("Door",2,0,0,0);
}


void MachineCalibration::on_pButton_door_set_clicked()
{
     if (predoorpos < ui->spinBox_Door->value())
        Sequence::getPtr()->actionDo("Door",4,ui->spinBox_Door->value()-predoorpos,0,0);
    else
        Sequence::getPtr()->actionDo("Door",3,predoorpos-ui->spinBox_Door->value(),0,0);

    predoorpos = ui->spinBox_Door->value();


}

void MachineCalibration::on_pButton_door_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("VDWorkX",ui->spinBox_Door->value());
    ui->spinBox_Door->setValue(ExGlobal::getPtr()->getCaliParam("VDWorkX"));
    Sequence::getPtr()->checkParam(true);
 }


void MachineCalibration::on_pButton_push_hard_clicked()
{
    ui->spinBox_Pump->setValue(0);
    Sequence::getPtr()->actionDo("Pump",1,0,0,0);
 }

void MachineCalibration::on_pButton_push_soft_clicked()
{
    ui->spinBox_Pump->setValue(ExGlobal::getPtr()->getCaliParam("PumpSoftHomeX"));
    Sequence::getPtr()->actionDo("Pump",9,ui->spinBox_Pump->value(),0,0);
    predoorpos = ui->spinBox_Pump->value();
}

void MachineCalibration::on_pButton_push_work_clicked()
{
    ui->spinBox_Pump->setValue(ExGlobal::getPtr()->getCaliParam("PumpToolHomeX"));
    Sequence::getPtr()->actionDo("Pump",9,ui->spinBox_Pump->value(),0,0);
}


void MachineCalibration::on_pButton_push_set_clicked()
{
    Sequence::getPtr()->actionDo("Pump",9,ui->spinBox_Pump->value(),0,0);
}

void MachineCalibration::on_pButton_push_save_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("PumpToolHomeX",ui->spinBox_Pump->value());
    ExGlobal::getPtr()->updateCaliParam("PumpSoftHomeX",ui->spinBox_Pump->value()-ExGlobal::getPtr()->getCaliParam("PumpSoftHomeOffset"));
    ui->spinBox_Pump->setValue(ExGlobal::getPtr()->getCaliParam("PumpToolHomeX"));
    Sequence::getPtr()->checkParam(true);
}


void MachineCalibration::on_pButton_door_in_clicked()
{
    Sequence::getPtr()->actionDo("Door",4,ui->spinBox_door_step->value(),ui->spinBox_speed->value(),0);
 }

void MachineCalibration::on_pButton_door_out_clicked()
{
    Sequence::getPtr()->actionDo("Door",3,ui->spinBox_door_step->value(),ui->spinBox_speed->value(),0);
}
