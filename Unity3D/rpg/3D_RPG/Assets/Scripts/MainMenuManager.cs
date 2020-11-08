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
        if (id.text == "" || password.text == "")
        {
            notify.text = "아이디 또는 비밀번호를 입력해주세요.";
            return;
        }
        if (GameManager.instance.Id_JoongBok_Check(id.text))
        {
            if (GameManager.instance.Password_Check(id.text, password.text))
            {
                GameManager.instance.SetCurUserData(id.text);
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
        if(id.text == "" || password.text == "")
        {
            notify.text = "아이디 또는 비밀번호를 입력해주세요.";
            return;
        }
        if(GameManager.instance.AddNewUsersData(id.text, password.text))
        {
            SceneManager.LoadScene("Town");
        }
        else
        {
            notify.text = "이미 존재하는 아이디 입니다.";
        }
    }

    public void ExitGame()
    {
        Application.Quit();
    }
}
