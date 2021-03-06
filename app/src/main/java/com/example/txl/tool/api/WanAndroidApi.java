package com.example.txl.tool.api;

import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import okhttp3.Cookie;
import okhttp3.CookieJar;
import okhttp3.HttpUrl;
import okhttp3.OkHttpClient;
import retrofit2.Call;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;
import retrofit2.http.Field;
import retrofit2.http.FormUrlEncoded;
import retrofit2.http.GET;
import retrofit2.http.POST;
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

    private static ConcurrentHashMap<String, List<Cookie>> cookieStore = new ConcurrentHashMap<>();
    public static IWanAndroidAPI getiWanAndroidAPI(){
        if(mIWanAndroidAPI == null){
            OkHttpClient okHttpClient = new OkHttpClient.Builder()
                    .cookieJar(new CookieJar() {
                        @Override
                        public void saveFromResponse(HttpUrl url, List<Cookie> cookies) {
                            for (Cookie cookie : cookies)
                            {
                                System.out.println("cookies: " + cookie.toString());
                            }
                            cookieStore.put(baseWanAndroidUrl,cookies);
                        }

                        @Override
                        public List<Cookie> loadForRequest(HttpUrl url) {
                            List<Cookie> cookies = cookieStore.get(url.host());
                            return cookies != null ? cookies : new ArrayList<Cookie>();
                        }
                    })
                    .build();
            Retrofit retrofit = new Retrofit.Builder()
                    .baseUrl(baseWanAndroidUrl)
                    .addConverterFactory(GsonConverterFactory.create())
                    .client(okHttpClient)
                    .build();
            mIWanAndroidAPI = retrofit.create(IWanAndroidAPI.class);
        }
        return mIWanAndroidAPI;
    }

    public interface IWanAndroidAPI{

        /**
         * Retrofit ????????????
         * ??????wanAndroid??????banner
         * */
        @GET("banner/json")
        Call<JSONObject> getBanner();

        /**
         * url????????????????????????????????????????????????url????????????{}??????????????????????????????????????? @Path ????????????
         * ?????????????????????????????????{}??????????????????
         *??????????????????
         * */
        @GET("article/list/{page}/json")
        Call<JSONObject> getArticleList(@Path("page") int page);

        /**
         * ???????????????@QueryMap ?????????????????????????????????
         * ????????????????????????
         * */
        @GET("article/list/{page}/json")
        Call<JSONObject> getKnowledgeSystemAArticleList(@Path("page") int page, @Query("cid") int cid);

        /**
         * ??????@Query ??????????????????
         * */
        @GET("group/{id}/users")
        Call<JSONObject> groupList(@Path("id") int groupId, @QueryMap Map<String, String> options);

        @FormUrlEncoded
        @POST("user/login")
        Call<JSONObject> userLogin(@Field("username") String username, @Field("password") String password);
    }
}
