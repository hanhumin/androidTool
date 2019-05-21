package com.example.txl.tool.design.pattern.Memomode;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/22
 * description：
 */
public class Caretaker {
    Memoto memoto;

    public void archive(Memoto memoto){
        this.memoto = memoto;
    }

    public Memoto getMemoto(){
        return memoto;
    }
}
