// Fill out your copyright notice in the Description page of Project Settings.


#include "DataExporter.h"
#include <iostream>


void UDataExporter::Load(FString& FilePath) {
    FString Content;

    if (!FFileHelper::LoadFileToString(Content, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load OpenDrive file '%s'"), *FilePath);
    }
    MainDoc.load_string(TCHAR_TO_ANSI(*Content));
}

void UDataExporter::AddTrafficLightGroups(TArray<FEditorTrafficLightGroupData> TrafficLightGroups){
    pugi::xml_node TopNode = MainDoc.child("OpenDRIVE");
    for (FEditorTrafficLightGroupData CurrentTFGData : TrafficLightGroups) {
        pugi::xml_node tg = TopNode.append_child("tlGroup");
        tg.append_attribute("redTime") = CurrentTFGData.RedTime;
        tg.append_attribute("yellowTime") = CurrentTFGData.YellowTime;
        tg.append_attribute("greenTime") = CurrentTFGData.GreenTime;
        for(FEditorTrafficLightData& CurrentTLData : CurrentTFGData.TrafficLights){
            pugi::xml_node tg_trafficlights = tg.append_child("trafficlight");
            tg_trafficlights.append_attribute("xPos") = CurrentTLData.Location.X;
            tg_trafficlights.append_attribute("yPos") = CurrentTLData.Location.Y;
            tg_trafficlights.append_attribute("zPos") = CurrentTLData.Location.Z;
            tg_trafficlights.append_attribute("xRot") = CurrentTLData.Rotation.Roll;
            tg_trafficlights.append_attribute("yRot") = CurrentTLData.Rotation.Pitch;
            tg_trafficlights.append_attribute("zRot") = CurrentTLData.Rotation.Yaw;
            tg_trafficlights.append_attribute("scale") = CurrentTLData.Scale.X;
            pugi::xml_node tg_tfBox = tg_trafficlights.append_child("tfBox");
            tg_tfBox.append_attribute("xPos") = CurrentTLData.TriggerBoxLocation.X;
            tg_tfBox.append_attribute("yPos") = CurrentTLData.TriggerBoxLocation.Y;
            tg_tfBox.append_attribute("zPos") = CurrentTLData.TriggerBoxLocation.Z;
            tg_tfBox.append_attribute("xRot") = CurrentTLData.TriggerBoxRotation.Roll;
            tg_tfBox.append_attribute("yRot") = CurrentTLData.TriggerBoxRotation.Pitch;
            tg_tfBox.append_attribute("zRot") = CurrentTLData.TriggerBoxRotation.Yaw;
            tg_tfBox.append_attribute("scale") = CurrentTLData.TriggerBoxScale.X;
            tg_tfBox.append_attribute("xExtent") = CurrentTLData.TriggerBoxExtent.X;
            tg_tfBox.append_attribute("yExtent") = CurrentTLData.TriggerBoxExtent.Y;
            tg_tfBox.append_attribute("zExtent") = CurrentTLData.TriggerBoxExtent.Z;
        }
    }
}

void UDataExporter::AddTrafficSigns(TArray<FEditorSpeedSignData> TrafficSigns) {
    pugi::xml_node TopNode = MainDoc.child("OpenDRIVE");
    for(FEditorSpeedSignData& CurrentSSData : TrafficSigns) {
        pugi::xml_node trafficsign = TopNode.append_child("trafficsign");
        trafficsign.append_attribute("xPos") = CurrentSSData.Location.X;
        trafficsign.append_attribute("yPos") = CurrentSSData.Location.Y;
        trafficsign.append_attribute("zPos") = CurrentSSData.Location.Z;
        trafficsign.append_attribute("xRot") = CurrentSSData.Rotation.Roll;
        trafficsign.append_attribute("yRot") = CurrentSSData.Rotation.Pitch;
        trafficsign.append_attribute("zRot") = CurrentSSData.Rotation.Yaw;
        trafficsign.append_attribute("speed") = CurrentSSData.Speed;
        pugi::xml_node ts_tfBox = trafficsign.append_child("tsBox");
        ts_tfBox.append_attribute("xPos") = CurrentSSData.TriggerBoxLocation.X;
        ts_tfBox.append_attribute("yPos") = CurrentSSData.TriggerBoxLocation.Y;
        ts_tfBox.append_attribute("zPos") = CurrentSSData.TriggerBoxLocation.Z;
        ts_tfBox.append_attribute("xRot") = CurrentSSData.TriggerBoxRotation.Roll;
        ts_tfBox.append_attribute("yRot") = CurrentSSData.TriggerBoxRotation.Pitch;
        ts_tfBox.append_attribute("zRot") = CurrentSSData.TriggerBoxRotation.Yaw;
        ts_tfBox.append_attribute("scale") = CurrentSSData.TriggerBoxScale.X;
        ts_tfBox.append_attribute("xExtent") = CurrentSSData.TriggerBoxExtent.X;
        ts_tfBox.append_attribute("yExtent") = CurrentSSData.TriggerBoxExtent.Y;
        ts_tfBox.append_attribute("zExtent") = CurrentSSData.TriggerBoxExtent.Z;
    }
}

void UDataExporter::Save(FString& FilePath) {
    MainDoc.save_file(TCHAR_TO_ANSI(*FilePath));
}
