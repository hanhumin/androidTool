package com.example.txl.tool.mediaprovider.Interactive;

import com.example.txl.tool.mediaprovider.MediaInfo;

import java.util.List;

/**
 * 查询所有包含当前类型的文件夹
 * 查询某个文件夹下的当前资源
 * 查询所有当前文件
 * */
public interface IQueryStrategy {
    /***
     * 查询所有包含当前类型资源的文件夹
     * */
    List<MediaInfo> queryAllDir();
    /**
     * 查询所有当前类型的资源
     * */
    List<MediaInfo> queryAllFile();
    /**
     * 查询指定父文件夹id下的所有当前类型的资源
     * */
    List<MediaInfo> queryAllFileByPid(long pid);
}
