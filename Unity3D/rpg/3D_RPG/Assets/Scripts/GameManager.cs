using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class GameManager : MonoBehaviour
{
    static public GameManager instance = null;
    private PlayerStatus ps;

    public bool is_loaded = false;
    public user_data load_data;
    public Dictionary<string, user_data> user_data_dict;

    [System.Serializable]
    public struct user_data
    {
        public string id;
        public string password;
        public int level;
        public float attack;
        public float defense;
        public float curExp;
        public float maxExp;
        public float curHp;
        public float maxHp;
        public float curMp;
        public float maxMp;
        public ItemData weapon;
        public ItemData armor;
        public ItemData[] inventory;
        public user_data(string id, string password)
        {
            this.id = id;
            this.password = password;
            level = 1;
            attack = 10;
            defense = 0;
            curExp = 0;
            maxExp = 100;
            curHp = 100;
            maxHp = 100;
            curMp = 100;
            maxMp = 100;
            weapon = null;
            armor = null;
            inventory = new ItemData[InventorySystem.instance.itemSlots.Length];
            for (int i = 0; i < InventorySystem.instance.itemSlots.Length; i++)
            {
                inventory[i] = null;
            }
        }
    }
    public List<user_data> users_data_list;

    [System.Serializable]
    public struct SaveData
    {
        public user_data[] users_data_arr;
    }
    public SaveData sd;
    private void Awake()
    {
        if (instance == null)
        {
            users_data_list = new List<user_data>();
            user_data_dict = new Dictionary<string, user_data>();
            string path = Application.dataPath + "/UsersData/usersData" + ".json";
            string jsonData = File.ReadAllText(path);
            sd = JsonUtility.FromJson<SaveData>(jsonData);

            for (int i = 0; i < sd.users_data_arr.Length; i++)
            {
                users_data_list.Add(sd.users_data_arr[i]);
                user_data_dict.Add(sd.users_data_arr[i].id, sd.users_data_arr[i]);
            }
            instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }
    }

    private void Start()
    {

    }

    public bool AddNewUsersData(string id, string password)
    {
        if (GameManager.instance.user_data_dict.ContainsKey(id))
        {
            return false;
        }
        GameManager.user_data temp = new GameManager.user_data(id, password);
        SaveUserData(temp);
        GameManager.instance.user_data_dict.Add(id, temp);
        GameManager.instance.load_data = GameManager.instance.user_data_dict[id];
        GameManager.instance.is_loaded = true;
        return true;
    }

    public void SaveUserData(GameManager.user_data temp)
    {
        GameManager.instance.users_data_list.Add(temp);
        sd.users_data_arr = new user_data[users_data_list.Count];
        for (int i = 0; i < users_data_list.Count; i++)
        {
            sd.users_data_arr[i] = users_data_list[i];
        }
        string jsonData = JsonUtility.ToJson(sd, true);
        string path = Application.dataPath + "/UsersData/usersData" + ".json";
        File.WriteAllText(path, jsonData);
    }

    void ExitGame()
    {
        user_data temp = new user_data(load_data.id, load_data.password);
        ps = GameObject.Find("Player").GetComponent<PlayerStatus>();
        temp.level = ps.level;
        temp.maxExp = ps.maxExp;
        temp.curExp = ps.curExp;
        temp.maxHp = ps.maxHp;
        temp.curHp = ps.curHp;
        temp.maxMp = ps.maxMp;
        temp.curMp = ps.curMp;
        temp.attack = ps.attack;
        temp.defense = ps.defense;
        temp.weapon = CharacterInfoSystem.instance.weaponSlot.item;
        temp.armor = CharacterInfoSystem.instance.armorSlot.item;
        for(int i = 0; i < InventorySystem.instance.itemSlots.Length; i++)
        {
            temp.inventory[i] = InventorySystem.instance.itemSlots[i].item;
        }
        user_data_dict[temp.id] = temp;
    }
}
