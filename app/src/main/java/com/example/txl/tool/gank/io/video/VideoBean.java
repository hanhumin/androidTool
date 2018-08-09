package com.example.txl.tool.gank.io.video;

import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/7
 * description：
 */
public class VideoBean {
    boolean error;
    List<VideoInfo> results;

    public boolean isError() {
        return error;
    }

    public void setError(boolean error) {
        this.error = error;
    }

    public List<VideoInfo> getResults() {
        return results;
    }

    public void setResults(List<VideoInfo> results) {
        this.results = results;
    }

    public class VideoInfo{
        String _id;
        String createdAt;
        String desc;
        String publishedAt;
        String source;
        String type;
        String url;
        String used;
        String who;
        VideoContent content;

        public String get_id() {
            return _id;
        }

        public void set_id(String _id) {
            this._id = _id;
        }

        public String getCreatedAt() {
            return createdAt;
        }

        public void setCreatedAt(String createdAt) {
            this.createdAt = createdAt;
        }

        public String getDesc() {
            return desc;
        }

        public void setDesc(String desc) {
            this.desc = desc;
        }

        public String getPublishedAt() {
            return publishedAt;
        }

        public void setPublishedAt(String publishedAt) {
            this.publishedAt = publishedAt;
        }

        public String getSource() {
            return source;
        }

        public void setSource(String source) {
            this.source = source;
        }

        public String getType() {
            return type;
        }

        public void setType(String type) {
            this.type = type;
        }

        public String getUrl() {
            return url;
        }

        public void setUrl(String url) {
            this.url = url;
        }

        public String getUsed() {
            return used;
        }

        public void setUsed(String used) {
            this.used = used;
        }

        public String getWho() {
            return who;
        }

        public void setWho(String who) {
            this.who = who;
        }

        public VideoContent getContent() {
            return content;
        }

        public void setContent(VideoContent content) {
            this.content = content;
        }

        public class VideoContent{
            int hasData;
            int videoWidth;
            int videoHeight;
            String playAddr;
            String cover;

            public int getHasData() {
                return hasData;
            }

            public void setHasData(int hasData) {
                this.hasData = hasData;
            }

            public int getVideoWidth() {
                return videoWidth;
            }

            public void setVideoWidth(int videoWidth) {
                this.videoWidth = videoWidth;
            }

            public int getVideoHeight() {
                return videoHeight;
            }

            public void setVideoHeight(int videoHeight) {
                this.videoHeight = videoHeight;
            }

            public String getPlayAddr() {
                return playAddr;
            }

            public void setPlayAddr(String playAddr) {
                this.playAddr = playAddr;
            }

            public String getCover() {
                return cover;
            }

            public void setCover(String cover) {
                this.cover = cover;
            }
        }
    }
}
