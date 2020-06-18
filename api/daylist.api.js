/*
 * @Author: your name
 * @Date: 2020-06-18 09:09:16
 * @LastEditTime: 2020-06-18 10:25:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/api/daylist.api.js
 */ 
export const UserItem = 
{
    name: String,
    sex: Number, // 1：女 0：男
    age: Number,
    account: String,
    cipher: String
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
}