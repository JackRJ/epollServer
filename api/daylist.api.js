/*
 * @Author: your name
 * @Date: 2020-06-18 09:09:16
 * @LastEditTime: 2020-06-20 09:53:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/api/daylist.api.js
 */ 

/**
 * 用户信息
 */
export const UserItem = 
{
    id: Number,
    name: String,
    sex: Number, // 1：女 0：男
    age: Number,
    account: String,
    cipher: String
}

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
     */
    'daylist/login' = 
    {
        method: 'POST',
        req: [account, cipher],
        res: 
        {
            result: String,
            msg: String
        }
    }

    /**
     * 注册
     */
    'daylist/register' = 
    {
        method: 'POST',
        req: [account, cipher],
        res: 
        {
            result: String,
            msg: String
        }
    }

    /**
     * 用户上传日程
     */
    'daylist/uploadScheduleItem' = 
    {
        method: 'POST',
        req: [ScheduleItem],
        res: 
        {
            result: String,
            msg: String
        }
    }
}