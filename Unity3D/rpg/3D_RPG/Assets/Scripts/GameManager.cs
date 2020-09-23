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
    public struct slot_data
    {
        public int item_id;
        public int item_nums;
        public slot_data(int id, int nums)
        {
            this.item_id = id;
            this.item_nums = nums;
        }
    }

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
        public int weapon;
        public int armor;
        public slot_data[] inventory;
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
            weapon = 0;
            armor = 0;
            inventory = new slot_data[42];
            for (int i = 0; i < 42; i++)
            {
                inventory[i] = new slot_data(0,0);
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
        if (user_data_dict.ContainsKey(id))
        {
            return false;
        }
        user_data temp = new user_data(id, password);
        users_data_list.Add(temp);
        sd.users_data_arr = new user_data[users_data_list.Count];
        for (int i = 0; i < users_data_list.Count; i++)
        {
            sd.users_data_arr[i] = users_data_list[i];
        }
        user_data_dict.Add(id, temp);
        load_data = instance.user_data_dict[id];
        is_loaded = true;
        SaveUserData();
        return true;
    }

    public void SaveUserData()
    {
        string jsonData = JsonUtility.ToJson(sd, true);
        string path = Application.dataPath + "/UsersData/usersData" + ".json";
        File.WriteAllText(path, jsonData);
    }

    public void ExitGame()
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
        if(CharacterInfoSystem.instance.weaponSlot.item_id != 0)
        {
            temp.attack -= ItemSystem.instance.weapon_dict[CharacterInfoSystem.instance.weaponSlot.item_id].attack;
        }
        temp.defense = ps.defense;
        if(CharacterInfoSystem.instance.armorSlot.item_id != 0)
        {
            temp.defense -= ItemSystem.instance.armor_dict[CharacterInfoSystem.instance.armorSlot.item_id].defense;
        }
        temp.weapon = CharacterInfoSystem.instance.weaponSlot.item_id;
        temp.armor = CharacterInfoSystem.instance.armorSlot.item_id;
        for (int i = 0; i < InventorySystem.instance.itemSlots.Length; i++)
        {
            temp.inventory[i].item_id = InventorySystem.instance.itemSlots[i].item_id;
            temp.inventory[i].item_nums = InventorySystem.instance.itemSlots[i].item_nums;
        }
        for (int i = 0; i < sd.users_data_arr.Length; i++)
        {
            if(sd.users_data_arr[i].id == temp.id)
            {
                sd.users_data_arr[i] = temp;
            }
        }
        user_data_dict[temp.id] = temp;
        SaveUserData();
        Application.Quit();
    }
}
