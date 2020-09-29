using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkillCoolTimeSystem : MonoBehaviour
{
    public SkillCoolTime[] skillCools;
    public static SkillCoolTimeSystem instance = null;

    private void Awake()
    {
        if(instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(gameObject);
        }
    }
    // Start is called before the first frame update
    void Start()
    {
           
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
