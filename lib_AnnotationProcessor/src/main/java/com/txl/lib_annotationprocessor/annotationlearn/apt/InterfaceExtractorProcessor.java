package com.txl.lib_annotationprocessor.annotationlearn.apt;


import com.sun.mirror.apt.AnnotationProcessor;
import com.sun.mirror.apt.AnnotationProcessorEnvironment;
import com.sun.mirror.declaration.MethodDeclaration;
import com.sun.mirror.declaration.Modifier;
import com.sun.mirror.declaration.ParameterDeclaration;
import com.sun.mirror.declaration.TypeDeclaration;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/6/1
 * description：
 */
public class InterfaceExtractorProcessor implements AnnotationProcessor {
    private final AnnotationProcessorEnvironment env;
    private ArrayList<MethodDeclaration> interfaceMethods = new ArrayList<>();

    public InterfaceExtractorProcessor(AnnotationProcessorEnvironment env) {
        this.env = env;
    }

    @Override
    public void process() {
        System.out.println("开始执行注解程序");
        for (TypeDeclaration typeDeclaration : env.getSpecifiedTypeDeclarations()){
            ExtractInterface annot = typeDeclaration.getAnnotation(ExtractInterface.class);
            if(annot == null){
                break;
            }
            for (MethodDeclaration m :typeDeclaration.getMethods()){
                if(m.getModifiers().contains(Modifier.PUBLIC) && !(m.getModifiers().contains(Modifier.STATIC))){
                    interfaceMethods.add(m);
                }
            }
            if(interfaceMethods.size() > 0){
                try {
                    PrintWriter writer  = env.getFiler().createSourceFile(annot.value());
                    writer.println("package "+typeDeclaration.getPackage().getQualifiedName()+";");
                    writer.println("public interface "+annot.value() +"{");
                    for (MethodDeclaration m:interfaceMethods){
                        writer.print("  public ");
                        writer.print(m.getReturnType() + " ");
                        writer.print(m.getSimpleName() + " (");
                        int i = 0;
                        for (ParameterDeclaration parm : m.getParameters()){
                            writer.print(parm.getType()  + " "+parm.getSimpleName());
                            if(++i < m.getParameters().size()){
                                writer.print(", ");
                            }
                        }
                        writer.println(");");
                    }
                    writer.println("}");
                    writer.close();
                } catch (IOException e) {
                    e.printStackTrace();
                    throw new RuntimeException(e);
                }
            }
        }
    }
}
