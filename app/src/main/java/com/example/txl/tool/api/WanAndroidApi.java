package com.example.txl.tool.api;

import org.json.JSONObject;

import java.util.Map;

import okhttp3.OkHttpClient;
import retrofit2.Call;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;
import retrofit2.http.GET;
import retrofit2.http.Path;
import retrofit2.http.Query;
import retrofit2.http.QueryMap;

/**
 * @author TXL
 * description :
 */
public class WanAndroidApi {

    private static final String baseWanAndroidUrl = "https://www.wanandroid.com/";

    private static IWanAndroidAPI mIWanAndroidAPI;

    public static IWanAndroidAPI getiWanAndroidAPI(){
        if(mIWanAndroidAPI == null){
            Retrofit retrofit = new Retrofit.Builder()
                    .baseUrl(baseWanAndroidUrl)
                    .addConverterFactory(GsonConverterFactory.create())
                    .build();
            mIWanAndroidAPI = retrofit.create(IWanAndroidAPI.class);
        }
        return mIWanAndroidAPI;
    }

    interface IWanAndroidAPI{

        /**
         * Retrofit 基本使用
         * 获取wanAndroid首页banner
         * */
        @GET("banner/json")
        Call<JSONObject> getBanner();

        /**
         * url可以通过参数动态替换，需要替换的url字段使用{}包裹起来，同时在参数中使用 @Path 进行注解
         * 并且其中的字符串必须和{}中的内容相同
         *首页文章列表
         * */
        @GET("article/list/{page}/json")
        Call<JSONObject> getArticleList(@Path("page") int page);

        /**
         * 也可以通过@QueryMap 添加一系列要查询的参数
         * 知识体系下的文章
         * */
        @GET("article/list/{page}/json")
        Call<JSONObject> getKnowledgeSystemAArticleList(@Path("page") int page, @Query("cid") int cid);

        /**
         * 通过@Query 添加查询参数
         * */
        @GET("group/{id}/users")
        Call<JSONObject> groupList(@Path("id") int groupId, @QueryMap Map<String, String> options);
    }
}
