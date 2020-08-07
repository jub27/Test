using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MainMenu : MonoBehaviour
{
    public GameObject mainMenu;
    public GameObject loadList;
    public SaveLoadSlotInfo[] loadSlots;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void NewGame()
    {
        SceneManager.LoadScene("Main");
    }

    public void OpenLoadList()
    {
        for (int i = 0; i < 5; i++)
        {
            loadSlots[i].SetSlotInfo(i);
        }
        mainMenu.SetActive(false);
        loadList.SetActive(true);
    }

    public void LoadGame(int index)
    {
        if (loadSlots[index].empty)
            return;
        Variables.isLoaded = true;
        Variables.loadIndex = index;
        SceneManager.LoadScene("Main");
    }

    public void LoadBack()
    {
        loadList.SetActive(false);
        mainMenu.SetActive(true);
    }

    public void Exit()
    {
        Application.Quit();
    }
}