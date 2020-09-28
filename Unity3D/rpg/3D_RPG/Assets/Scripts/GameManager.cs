using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class GameManager : MonoBehaviour
{
    static public GameManager instance = null;
    private PlayerStatus ps;
    public user_data cur_user_data;

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
        public int gold;
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
            gold = 0;
            inventory = new slot_data[42];
            for (int i = 0; i < 42; i++)
            {
                inventory[i] = new slot_data(0,0);
            }
        }
    }

    [System.Serializable]
    public struct SaveData
    {
        public user_data[] users_data_arr;
    }
    public SaveData sd; // json으로 저장할 전체 유저들 데이터
    public Dictionary<string, user_data> user_data_dict; // 로그인 할때 id, password 참조할 dictionary
    public List<user_data> users_data_list; // 새로 캐릭터 생성할때마다 이 리스트에 저장, json으로 저장할때는 SaveData 형태로 바꿔서 저장
    private void Awake()// json 데이터 읽어와서 유저 데이터, list, dictionary 초기화
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

    public bool AddNewUsersData(string id, string password) // 새로 생성한 유저를 dictionary에 추가하고 json형태로 저장,성공하면 true, 실패하면 false 반환
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
        cur_user_data = instance.user_data_dict[id];
        string jsonData = JsonUtility.ToJson(sd, true);
        string path = Application.dataPath + "/UsersData/usersData" + ".json";
        File.WriteAllText(path, jsonData);
        return true;
    }

    public void SaveCurUserData() // 현재 유저 데이터를 json 으로 저장
    {
        user_data temp = new user_data(cur_user_data.id, cur_user_data.password);
        ps = GameObject.Find("Player").GetComponent<PlayerStatus>();
        temp.level = ps.level;
        temp.maxExp = ps.maxExp;
        temp.curExp = ps.curExp;
        temp.maxHp = ps.maxHp;
        temp.curHp = ps.curHp;
        temp.maxMp = ps.maxMp;
        temp.curMp = ps.curMp;
        temp.attack = ps.attack;
        if (CharacterInfoSystem.instance.weaponSlot.item_id != 0)
        {
            temp.attack -= ItemSystem.instance.weapon_dict[CharacterInfoSystem.instance.weaponSlot.item_id].attack;
        }
        temp.defense = ps.defense;
        if (CharacterInfoSystem.instance.armorSlot.item_id != 0)
        {
            temp.defense -= ItemSystem.instance.armor_dict[CharacterInfoSystem.instance.armorSlot.item_id].defense;
        }
        temp.weapon = CharacterInfoSystem.instance.weaponSlot.item_id;
        temp.armor = CharacterInfoSystem.instance.armorSlot.item_id;
        temp.gold = ps.gold;
        for (int i = 0; i < InventorySystem.instance.itemSlots.Length; i++)
        {
            temp.inventory[i].item_id = InventorySystem.instance.itemSlots[i].item_id;
            temp.inventory[i].item_nums = InventorySystem.instance.itemSlots[i].item_nums;
        }
        for (int i = 0; i < sd.users_data_arr.Length; i++)
        {
            if (sd.users_data_arr[i].id == temp.id)
            {
                sd.users_data_arr[i] = temp;
            }
        }
        user_data_dict[temp.id] = temp;
        string jsonData = JsonUtility.ToJson(sd, true);
        string path = Application.dataPath + "/UsersData/usersData" + ".json";
        File.WriteAllText(path, jsonData);
    }

    public void ExitGame() // ESC - 게임 종료 버튼을 눌렀을때 수행됨
    {
        SaveCurUserData();
        Application.Quit();
    }
}
