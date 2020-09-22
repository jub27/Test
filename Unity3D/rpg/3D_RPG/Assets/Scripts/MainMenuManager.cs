using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.IO;
using UnityEngine.SceneManagement;
public class MainMenuManager : MonoBehaviour
{
    public InputField id;
    public InputField password;
    public Text notify;

    public void Login()
    {
        if (GameManager.instance.user_data_dict.ContainsKey(id.text))
        {
            if (GameManager.instance.user_data_dict[id.text].password == password.text)
            {
                GameManager.instance.load_data = GameManager.instance.user_data_dict[id.text];
                GameManager.instance.is_loaded = true;
                SceneManager.LoadScene("Town");
            }
            else
            {
                notify.text = "아이디 혹은 비밀번호가 일치하지 않습니다.";
            }
        }
        else
        {
            notify.text = "아이디 혹은 비밀번호가 일치하지 않습니다.";
        }
    }

    public void AddNewUser()
    {
        if(GameManager.instance.AddNewUsersData(id.text, password.text))
        {
            SceneManager.LoadScene("Town");
        }
        else
        {
            notify.text = "이미 존재하는 아이디 입니다.";
        }
    }
}
