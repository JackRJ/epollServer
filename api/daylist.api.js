/*
 * @Author: your name
 * @Date: 2020-06-18 09:09:16
 * @LastEditTime: 2020-07-01 23:40:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/api/daylist.api.js
 */ 

/**
 * 用户信息
 */
export const UserItem = 
{
    // id: Number,
    name: String,
    sex: Number, // 1：女 0：男
    age: Number,
    account: String,
    cipher: String
}

/**
 * 日程信息
 */
export const ScheduleItem = 
{
    scheduleId: Number,
    userId: Number,
    isAlarm: Number, // 1: 是 0：否
    advancedAlarmMintes: Number, // 提前多少分钟提醒
    startTime: String, // Timestamp eg:2020-05-20 00:00:00
    endTime: String,
    location: String,
    describtion: String,
    remarks: String,
}

export const url = 'http://119.23.174.48:8006/';

class UserAPI
{
    /**
     * 登陆
     * 再次或者注册后登陆时，可以带上 cookie（三天内有效, 目前还没有写入expires），如 cookie 无效，则再判断账号密码, 正确则更新 cookie
     */
    'daylist/login' = 
    {
        method: 'POST',
        req: [account, cipher] / [userId(带cookie)],
        res: 
        {
            status: String,
            msg: String,
            userId: Number
        }
    }

    /**
     * 注册
     * account: 10位数字
     * cipher: 8位数字字母及以上，至20位
     * 注册之后，会返回一个 cookie, cookie 带上 userId 即可直接免密登陆，cookie 三天内有效
     */
    'daylist/register' = 
    {
        method: 'POST',
        req: [account, cipher],
        res: 
        {
            status: String,
            msg: String,
            userId: Number
        }
    }

    /**
     * 用户上传日程，必须携带 cookie
     */
    'daylist/uploadScheduleItem' = 
    {
        method: 'POST',
        req: [ScheduleItem],
        res: 
        {
            status: String,
            msg: String,
            scheduleId: Number
        }
    }

    /**
     * 获取用户日程，每次10条，必须携带 cookie, page 从 0 开始
     */
    'daylist/getUserItems' = 
    {
        method: 'GET',
        req: [userId, page],
        res:
        {
            status: String,
            msg: String,
            more: Number, // 1: 是 2: 否
            scheduleItems: [ScheduleItem]
        }
    }

    /**
     * 获取用户信息，必须携带 cookie
     */
    'daylist/getUserInformation' = 
    {
        method: 'GET',
        req: [userId],
        res: 
        {
            status: String,
            msg: String,
            userInformation: UserItem
        }
    }

    /**
     * 修改用户信息，必须携带 cookie
     */
    'daylist/modifyUserInformation' = 
    {
        method: 'POST',
        req: [userId, (UserItem)], // userItem中需要修改的项目即可，数目至少一个
        res: 
        {
            status: String,
            msg: String
        }
    }

    /**
     * 删除日程，必须携带 cookie
     */
    'daylist/deleteScheduleItem' = 
    {
        method: 'POST',
        req: [userId, scheduleId],
        res:
        {
            status: String,
            msg: String
        }
    }
}