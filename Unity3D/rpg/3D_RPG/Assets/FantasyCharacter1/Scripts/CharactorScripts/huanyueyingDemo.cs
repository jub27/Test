using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class caopiDemo : MonoBehaviour {

    public GameObject attackBullet;
    public GameObject magicBullet;
    public GameObject magic2Bullet;
    public GameObject ultimateBullet;
    public GameObject damageEffect1;
    public GameObject damageEffect2;
    public GameObject damageEffect3;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    IEnumerator delayBullet()
    {
        int count = 10;
        for (int i = 0; i < count; i++)
        {
            AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
            GameObject obj = GameObject.Instantiate(ultimateBullet);
            CurvelBullet2 bullet = obj.GetComponent<CurvelBullet2>();
            bullet.player = transform;
            bullet.target = c.transform;
           
            bullet.bulleting();
            yield return null;
            if(i % 9 == 0)
            {
                bullet.effectObj = damageEffect1;
                c.attacked();
                if (damageEffect2 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
            }
        }
    }

    IEnumerator delayBullet1()
    {
        int count = 10;
        for (int i = 0; i < count; i++)
        {
            AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
            GameObject obj = GameObject.Instantiate(magic2Bullet);
            CurvelBullet2 bullet = obj.GetComponent<CurvelBullet2>();
            bullet.player = transform;
            bullet.target = c.transform;

            bullet.bulleting();
            yield return null;
            if (i % 9 == 0)
            {
                bullet.effectObj = damageEffect1;
                c.attacked();
                if (damageEffect2 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
            }
        }
    }

    void preAction(string actionName)
    {
        AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
        string[] arr = actionName.Split('|');
        string name = arr[0];
        switch(name)
        {
            case AnimationName1.Attack:
                if(attackBullet != null)
                {
                    GameObject obj = GameObject.Instantiate(attackBullet);
                    NormalBullet1 bullet = obj.GetComponent<NormalBullet1>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect1;
                    bullet.bulleting();
                }
                break;
            case AnimationName1.Magic:
                if (attackBullet != null)
                {
                    GameObject obj = GameObject.Instantiate(attackBullet);
                    NormalBullet1 bullet = obj.GetComponent<NormalBullet1>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect1;
                    bullet.bulleting();


                }
                break;
            case AnimationName1.Magic2:
                if (attackBullet != null)
                {
                    GameObject obj = GameObject.Instantiate(attackBullet);
                    NormalBullet1 bullet = obj.GetComponent<NormalBullet1>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect2;
                    bullet.bulleting();
                }
                break;
            case AnimationName1.Ultimate:
                if (ultimateBullet != null)
                {
                    StartCoroutine(delayBullet());
                    StartCoroutine(delayBullet1());
                }
                if (damageEffect3 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect3);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                break;
        }
    }
}
