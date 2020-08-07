using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using UnityEngine.UI;

public class SaveLoadSlotInfo : MonoBehaviour
{
    public bool empty = true;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void SetSlotInfo(int index)
    {
        SaveData saveData = new SaveData();
        string path = Application.dataPath + "/SaveData/saveData" + index + ".json";
        string jsonData;
        try
        {
            jsonData = File.ReadAllText(path);
        }
        catch
        {
            GetComponentInChildren<Text>().text = "Empty";
            empty = true;
            return;
        }
        saveData = JsonUtility.FromJson<SaveData>(jsonData);
        GetComponentInChildren<Text>().text = "WAVE : " + saveData.waveData.wave + " / SCORE : " + saveData.score;
        empty = false;
    }
}
