package com.example.txl.tool.design.pattern.composite;

import java.util.ArrayList;
import java.util.List;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/28
 * description：
 */
public class Composite extends Component {
    /**
     * 存储节点的容器
     * */
    private List<Component> componentList = new ArrayList<>(  );

    public Composite(String name) {
        super( name );
    }

    @Override
    public void doSomething() {
        System.out.println( name );
        if(componentList != null){
            for (Component component : componentList){
                component.doSomething();
            }
        }
    }

    public void addChild(Component component){
        componentList.add( component );
    }

    public void removeChild(Component component){
        componentList.remove( component );
    }

    public Component getChild(int index){
        return componentList.get( index );
    }
}
