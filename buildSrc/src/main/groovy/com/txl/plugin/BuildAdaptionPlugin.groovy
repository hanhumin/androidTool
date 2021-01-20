package com.txl.plugin

import org.gradle.api.Plugin
import org.gradle.api.Project
import com.txl.plugin.task.ModuleAdaptionTask
import org.gradle.internal.impldep.org.apache.http.util.TextUtils


/**
 * 不仅提供创建适配的能力，还需要有地方删除对应的目录
 * */
class BuildAdaptionPlugin implements Plugin<Project> {
    def BuildAdaptionPluginExtension appExtension
    void apply(Project project) {
        appExtension = project.extensions.create('adaptionAppExtension', BuildAdaptionPluginExtension,project)//全局扩展属性
        appExtension.resPath = "${File.separator}src${File.separator}main${File.separator}res${File.separator}"
        appExtension.defaultDesignWidth = 360f
        appExtension.needToAdaptedWidth.add(411)
        handleProject(project.rootProject)


    }

    private void handleProject(Project project){
        def subProjects = project.subprojects
        for (item in subProjects){
            handleProject(item)
        }
        println("config project ${project.name}")
        def taskModuleAdaption = project.tasks.register("${project.name}BuildAdaption",ModuleAdaptionTask)
        taskModuleAdaption.configure{
            doFirst {
                def moduleExtensionProvider = appExtension.subAdaptionPluginExtensionMapProperty.getting(project.name)
                BuildAdaptionPluginExtension me = moduleExtensionProvider.orNull
                handleModuleAdaptionTaskProperty(taskModuleAdaption.get(),appExtension,me)
            }
        }
        project.afterEvaluate{
            try {
                project.getTasks()
                def preBuild = project.getTasks().getByName("preBuild")
                preBuild.configure {
                    dependsOn taskModuleAdaption
                }
            }catch(Exception e){
                e.printStackTrace()
            }
        }
    }

    static void handleModuleAdaptionTaskProperty(ModuleAdaptionTask task,BuildAdaptionPluginExtension appExtension,BuildAdaptionPluginExtension moduleExtension){
        task.needToAdaptedWidth.addAll(appExtension.needToAdaptedWidth.get())
        task.defaultDesignWidth = appExtension.defaultDesignWidth
        task.enableAdapter = appExtension.enableAdapter
        task.resPath = appExtension.resPath
        if(moduleExtension != null){
            if(moduleExtension.needToAdaptedWidth.orNull != null && !moduleExtension.needToAdaptedWidth.orNull.isEmpty()){
                task.needToAdaptedWidth = moduleExtension.needToAdaptedWidth.orNull
            }
            if(moduleExtension.defaultDesignWidth != 0){
                task.defaultDesignWidth = moduleExtension.defaultDesignWidth
            }
            task.enableAdapter = appExtension.enableAdapter && moduleExtension.enableAdapter
            if(!TextUtils.isEmpty(moduleExtension.resPath)){
                task.resPath = moduleExtension.resPath
            }
            if(moduleExtension.conversionMap.orNull != null && !moduleExtension.conversionMap.orNull.isEmpty()){
                task.conversionMap = moduleExtension.conversionMap.orNull
            }
        }
    }

}
