using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class EscMenuManager : MonoBehaviour
{
    public Button exitButton;
    private GameManager gm;

    void Start()
    {
        gm = GameObject.Find("GameManager").GetComponent<GameManager>();
    }

    public void OnExitButtonClick()
    {
        gm.ExitGame();
    }
}
