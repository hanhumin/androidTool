package com.example.txl.tool.task.dao;

import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/10/23
 * description：
 */
public interface ITaskDao<T> {

    /**
     * 创建表
     * */
    void createTable();

    /**
     * 查询所有任务
     * */
    List<T> queryAllTask();

    /**
     * 根据任务状态查询
     * */
    void queryTaskByStatus();

    /**
     * 添加任务
     * */
    void addTask(T task);

    /**
     * 更新任务
     * */
    void update(T task);

    /**
     * 删除
     * */
    void delete();
}
