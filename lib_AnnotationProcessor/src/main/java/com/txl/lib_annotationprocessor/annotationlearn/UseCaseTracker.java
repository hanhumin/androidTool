package com.txl.lib_annotationprocessor.annotationlearn;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/5/31
 * description：
 */
public class UseCaseTracker {
    public static void trackUseCases(List<Integer> useCases, Class<?> cl){
        for (Method m :cl.getDeclaredMethods()){
            UseCase useCase = m.getAnnotation(UseCase.class);
            if(useCase != null){
                System.out.println("Found UseCase : "+useCase.id()+" "+useCase.description());
                useCases.remove(Integer.valueOf(useCase.id()));
            }
        }
        for (int i : useCases){
            System.out.println("Warning : Missing use case -  "+i);
        }
    }

    public static void main(String[] args){
        List<Integer> useCases = new ArrayList<>();
        Collections.addAll(useCases,47,48,49,50);
        trackUseCases(useCases,PassWordUtils.class);
    }
}
