package com.txl.plugin.task

import org.gradle.api.DefaultTask
import org.gradle.api.tasks.Input
import org.gradle.api.tasks.TaskAction

/**
 * 每个模块进行适配的task
 * 每一个模块应该有下面的属性可以配置
 * 1.设计图 宽度（默认360）
 * 2.需要适配的尺寸，由总体一起配置，
 * 3.每一个需要适配尺寸的转换策略,默认是  缩放因子 * value中dimen的实际大小 （缩放因子=当前适配尺寸/设计图尺寸）
 * 4.每个模块可以自由开启或关闭适配，适配插件有总体开关
 * 5.设置res目录的路径
 * */
public class ModuleAdaptionTask extends DefaultTask{
    public static int CONVERSION_FACTOR_UN_SET = 0
    /**
     * 默认设计及图宽度
     * */
    @Input
    float defaultDesignWidth = 360f
    /**
     * 需要适配的最小宽度  比如 {400f,411f,480f}单位是dp，这个值不在每个模块单独设置，由总体配置来
     * */
    @Input
    List<Float> needToAdapted = new ArrayList<Float>()
    /**
     * 转换因子,默认不进行设置
     * */
    @Input
    Map<String,Integer> ConversionFactor

    /**
     * 是否开启适配
     * */
    @Input
    boolean enableAdapter = true
    /**
     * 资源路径，默认src/main/res
     * */
    @Input
    String resPath = ""

    @TaskAction
    def adaption(){
        println "start adaption"
        project
    }
}
