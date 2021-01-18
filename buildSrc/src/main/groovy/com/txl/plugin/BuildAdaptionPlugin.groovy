package com.txl.plugin

import com.txl.plugin.xmlutils.AndroidDimenXMLParser
import com.txl.plugin.xmlutils.FileUtil
import com.txl.plugin.xmlutils.FileX
import org.gradle.api.Plugin
import org.gradle.api.Project

class BuildAdaptionPlugin implements Plugin<Project> {
    void apply(Project project) {
        def extension = project.extensions.create('adaptionExtension', BuildAdaptionPluginExtension)
        // 创建一个任务
        project.tasks.create("buildAdaption") {
            doLast {
                println("doing TestPlugin doLast")
            }
            try {
                def rootProjectName = project.rootProject
                println("doing TestPlugin ${project.getProjectDir().path}")
                for (subProject in rootProjectName.childProjects.values()){
                    //强行指定路径来判断对应逻辑
                    String originFilePath = subProject.getProjectDir().path+File.separator+"src"+File.separator+"main"+File.separator+"res"+File.separator+"values"+File.separator+"dimens.xml"
                    println("subProject name ${subProject.name}  origin path ${originFilePath}")
                    FileX filex = new FileX(originFilePath)
                    if(!filex.exists()){
                        continue
                    }
                    Map<String,String> map = AndroidDimenXMLParser.readDimensXML(originFilePath)
                    String newFilePath = subProject.getProjectDir().path+File.separator+"src"+File.separator+"main"+File.separator+"res"+File.separator+"values-sw411dp"+File.separator+"dimens.xml"
                    filex = new FileX(newFilePath)
                    if(!filex.exists()){
                        FileUtil.createFile(newFilePath)
                    }
                    extension.properties
                    AndroidDimenXMLParser.saveMap2XML(map,newFilePath,360f,411f)
                    println("创建文件成功 $newFilePath")
                }
            } catch(IOException e1) {
                e1.printStackTrace()
            }

        }
    }


}
