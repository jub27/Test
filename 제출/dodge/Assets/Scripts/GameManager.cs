using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public GameObject gameoverText;
    public Text timeText;
    public Text recordText;

    private float surviveTime;
    private bool isGameOver;

    // Start is called before the first frame update
    void Start()
    {
        surviveTime = 0;
        isGameOver = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (!isGameOver)
        {
            surviveTime += Time.deltaTime;
            timeText.text = "Time :" + (int)surviveTime;
        }
        else
        {
            if (Input.GetKeyDown(KeyCode.R))
            {
                SceneManager.LoadScene("GameScene");
            }
        }
    }
    public void EndGame()
    {
        isGameOver = true;
        gameoverText.SetActive(true);

        float bestTIme = PlayerPrefs.GetFloat("BestTime");

        if(surviveTime > bestTIme)
        {
            bestTIme = surviveTime;
            PlayerPrefs.SetFloat("BestTime", bestTIme);
        }
        recordText.text = "BestTime: " + (int)bestTIme;
    }
}
