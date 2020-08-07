using UnityEngine;
using System.Collections.Generic;
using System.IO;
using UnityEngine.SceneManagement;
[System.Serializable]
public class PlayerData
{
    public float health;
    public int ammoRemain; // 남은 전체 탄약
    public int magCapacity; // 탄창 용량
    public int magAmmo; // 현재 탄창에 남아있는 탄약
    public Vector3 position;
    public Quaternion rotation;
    public PlayerData(float health, int ammoRemain, int magCapacity, int magAmmo, Vector3 position, Quaternion rotation)
    {
        this.health = health;
        this.ammoRemain = ammoRemain;
        this.magCapacity = magCapacity;
        this.magAmmo = magAmmo;
        this.position = position;
        this.rotation = rotation;
    }
}

[System.Serializable]
public class EnemyData
{ 
    public float startingHealth;
    public float health;
    public float damage;
    public float speed;
    public Color color;
    public Vector3 position;
    public Quaternion rotation;
    public EnemyData(float startingHealth, float health, float damage, float speed, Color color, Vector3 position, Quaternion rotation)
    {
        this.startingHealth = startingHealth;
        this.health = health;
        this.damage = damage;
        this.speed = speed;
        this.color = color;
        this.position = position;
        this.rotation = rotation;
    }
}

[System.Serializable]
public class WaveData
{
    public int wave;
    public EnemyData[] enemyDatas;
}

[System.Serializable]
public class SaveData
{
    public int score;
    public WaveData waveData;
    public PlayerData playerData;
}

// 점수와 게임 오버 여부를 관리하는 게임 매니저
public class GameManager : MonoBehaviour
{
    public GameObject optionWindow;
    public GameObject saveWindow;
    public SaveLoadSlotInfo[] saveSlots;
    public SaveLoadSlotInfo[] loadSlots;
    public GameObject loadWindow;
    public EnemySpawner enemySpawner;

    public enum GameState
    {
        RUN, PAUSE
    }

    public GameState gameState;

    private static GameManager m_instance; // 싱글톤이 할당될 static 변수
    public static GameManager instance // 싱글톤 접근용 프로퍼티
    {
        get
        {
            // 만약 싱글톤 변수에 아직 오브젝트가 할당되지 않았다면
            if (m_instance == null)
            {
                // 씬에서 GameManager 오브젝트를 찾아 할당
                m_instance = FindObjectOfType<GameManager>();
            }

            // 싱글톤 오브젝트를 반환
            return m_instance;
        }
    }

    public int score { get; private set; } // 현재 게임 점수
    public bool isGameover { get; private set; } // 게임 오버 상태

    private void Awake()
    {
        // 씬에 싱글톤 오브젝트가 된 다른 GameManager 오브젝트가 있다면
        if (instance != this)
        {
            // 자신을 파괴
            Destroy(gameObject);
        }
    }

    private void Start()
    {
        score = 0;
        // 플레이어 캐릭터의 사망 이벤트 발생시 게임 오버
        FindObjectOfType<PlayerHealth>().onDeath += EndGame;

        if (Variables.isLoaded)
        {
            SaveData saveData = new SaveData();
            string path = Application.dataPath + "/SaveData/saveData" + Variables.loadIndex + ".json";
            string jsonData = File.ReadAllText(path);
            saveData = JsonUtility.FromJson<SaveData>(jsonData);

            GameObject player = GameObject.Find("Player Character");

            Gun gun = GameObject.Find("Gun").GetComponent<Gun>();
            player.GetComponent<PlayerHealth>().health = saveData.playerData.health;
            player.GetComponent<PlayerHealth>().healthSlider.value = saveData.playerData.health;
            player.transform.position = saveData.playerData.position;
            player.transform.rotation = saveData.playerData.rotation;
            enemySpawner.wave = saveData.waveData.wave;
            score = saveData.score;
            UIManager.instance.UpdateScoreText(score);
            gun.magCapacity = saveData.playerData.magCapacity;
            gun.magAmmo = saveData.playerData.magAmmo;
            gun.ammoRemain = saveData.playerData.ammoRemain;

            for(int i = 0; i < saveData.waveData.enemyDatas.Length; i++)
            {
                enemySpawner.LoadEnemy(saveData.waveData.enemyDatas[i].startingHealth, saveData.waveData.enemyDatas[i].health, saveData.waveData.enemyDatas[i].damage,
                    saveData.waveData.enemyDatas[i].speed, saveData.waveData.enemyDatas[i].color, saveData.waveData.enemyDatas[i].position, saveData.waveData.enemyDatas[i].rotation);
            }

            Variables.isLoaded = false;
        }
        gameState = GameState.RUN;
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if (optionWindow.active)
            {
                CloseOptionWindow();
            }
            else
            {
                OpenOptionWindow();
            }
        }
    }

    // 점수를 추가하고 UI 갱신
    public void AddScore(int newScore)
    {
        // 게임 오버가 아닌 상태에서만 점수 증가 가능
        if (!isGameover)
        {
            // 점수 추가
            score += newScore;
            // 점수 UI 텍스트 갱신
            UIManager.instance.UpdateScoreText(score);
        }
    }

    // 게임 오버 처리
    public void EndGame()
    {
        // 게임 오버 상태를 참으로 변경
        isGameover = true;
        // 게임 오버 UI를 활성화
        UIManager.instance.SetActiveGameoverUI(true);
    }

    SaveData GetSaveData()
    {
        SaveData saveData = new SaveData();
        saveData.score = score;
        PlayerHealth ph = GameObject.Find("Player Character").GetComponent<PlayerHealth>();
        Gun gun = GameObject.Find("Gun").GetComponent<Gun>();
        PlayerData playerData = new PlayerData(ph.health, gun.ammoRemain, gun.magCapacity, gun.magAmmo, ph.gameObject.transform.position, ph.gameObject.transform.rotation);
        saveData.playerData = playerData;
        WaveData waveData = new WaveData();
        Enemy[] enemies = GameObject.FindObjectsOfType<Enemy>();
        waveData.enemyDatas = new EnemyData[enemies.Length];
        for (int i = 0; i < enemies.Length; i++)
        {
            waveData.enemyDatas[i] = new EnemyData(enemies[i].startingHealth, enemies[i].health,enemies[i].damage ,enemies[i].speed, enemies[i].color, enemies[i].transform.position, enemies[i].transform.rotation);
        }
        waveData.wave = GameObject.Find("EnemySpawner").GetComponent<EnemySpawner>().wave;
        saveData.waveData = waveData;
        return saveData;

    }

    public void SaveGame(int index)
    {
        SaveData saveData = GetSaveData();
        string jsonData = JsonUtility.ToJson(saveData, true);
        string path = Application.dataPath + "/SaveData/saveData" + index + ".json";
        File.WriteAllText(path, jsonData);
        saveWindow.SetActive(false);
        gameState = GameState.RUN;
        Time.timeScale = 1.0f;
    }

    public void LoadGame(int index)
    {
        if (loadSlots[index].empty)
            return;
        Variables.loadIndex = index;
        Variables.isLoaded = true;
        Time.timeScale = 1.0f;
        SceneManager.LoadScene("Main");
    }

    public void OpenOptionWindow()
    {
        if (isGameover)
            return;
        Time.timeScale = 0.0f;
        gameState = GameState.PAUSE;
        optionWindow.SetActive(true);
    }

    public void OpenSaveList()
    {
        for(int i = 0; i < 5; i++)
        {
            saveSlots[i].SetSlotInfo(i);
        }
        optionWindow.SetActive(false);
        saveWindow.SetActive(true);
    }

    public void OpenLoadList()
    {
        for (int i = 0; i < 5; i++)
        {
            loadSlots[i].SetSlotInfo(i);
        }
        optionWindow.SetActive(false);
        loadWindow.SetActive(true);
    }

    public void CloseOptionWindow()
    {
        Time.timeScale = 1.0f;
        optionWindow.SetActive(false);
        gameState = GameState.RUN;
    }

    public void SaveLoadBack()
    {
        saveWindow.SetActive(false);
        loadWindow.SetActive(false);
        optionWindow.SetActive(true);
    }

    public void OpenMainMenu()
    {
        SceneManager.LoadScene("MainMenu");
        Time.timeScale = 1.0f;
    }
}