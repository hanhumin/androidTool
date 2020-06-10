package com.txl.lib_annotationprocessor.annotationlearn.apt;

import javax.annotation.processing.AbstractProcessor;
import javax.annotation.processing.RoundEnvironment;
import javax.annotation.processing.SupportedAnnotationTypes;
import javax.annotation.processing.SupportedSourceVersion;
import javax.lang.model.SourceVersion;
import javax.lang.model.element.TypeElement;
import javax.tools.Diagnostic;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Set;

/**
 * Copyright (c) 2020 txl All rights reserved.
 * author：txl
 * date:2020/6/1
 * description:
 */
@SupportedAnnotationTypes({"com.txl.lib_annotationprocessor.annotationlearn.apt.ExtractInterface"})
@SupportedSourceVersion(SourceVersion.RELEASE_7)
public class InterfaceExtractorProcessorV2 extends AbstractProcessor {

    @Override
    public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment roundEnv) {
        System.out.println("start  deal custom annotation ! !");
        processingEnv.getMessager().printMessage(Diagnostic.Kind.NOTE,"message start  deal custom annotation");
        return true;
    }
}
