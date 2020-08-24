using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class huaxiongDemo1 : MonoBehaviour {

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

    IEnumerator delayBullet3()
    {
        int count = 12;
        List<RotateBullet2> list = new List<RotateBullet2>();
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(attackBullet);
            RotateBullet2 bullet = obj.GetComponent<RotateBullet2>();
            bullet.player = transform;
            bullet.effectObj = damageEffect1;
            bullet.bulleting();
            bullet.transform.parent = transform;
            bullet.r = 0.7f;
            bullet.y = 0.4f;
            list.Add(bullet);
            yield return new WaitForSeconds(0.25f);
        }
        yield return new WaitForSeconds(1.5f);
        for(int i = 0; i < list.Count; i++)
        {
            list[i].bulletState = BulletState1.wand;
            Vector3 pos1 = transform.position;
            pos1.y = list[i].transform.position.y;
            list[i].tarPos = MathUtil1.calcTargetPosByDis(pos1, list[i].transform.position, 10f);
        }
    }

    IEnumerator delayBullet4()
    {
        int count = 12;
        List<RotateBullet2> list = new List<RotateBullet2>();
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(attackBullet);
            RotateBullet2 bullet = obj.GetComponent<RotateBullet2>();
            bullet.player = transform;
            bullet.effectObj = damageEffect1;
            bullet.transform.parent = transform;
            bullet.bulleting();
            bullet.r = 0.7f;
            bullet.y = 0.2f;
            list.Add(bullet);
            yield return new WaitForSeconds(0.25f);
        }
        for (int i = 0; i < list.Count; i++)
        {
            list[i].bulletState = BulletState1.wand;
            Vector3 pos1 = transform.position;
            pos1.y = list[i].transform.position.y;
            list[i].tarPos = MathUtil1.calcTargetPosByDis(pos1, list[i].transform.position, 10f);
        }
    }

    IEnumerator delayBullet5()
    {
        int count = 12;
        List<RotateBullet2> list = new List<RotateBullet2>();
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(attackBullet);
            RotateBullet2 bullet = obj.GetComponent<RotateBullet2>();
            bullet.player = transform;
            bullet.effectObj = damageEffect1;
            bullet.transform.parent = transform;
            bullet.bulleting();

            bullet.r = 0.7f;
            bullet.y = 0.6f;
            list.Add(bullet);
            yield return new WaitForSeconds(0.15f);
        }
        for (int i = 0; i < list.Count; i++)
        {
            list[i].bulletState = BulletState1.wand;
            Vector3 pos1 = transform.position;
            pos1.y = list[i].transform.position.y;
            list[i].tarPos = MathUtil1.calcTargetPosByDis(pos1, list[i].transform.position, 10f);
        }
    }

    IEnumerator delayBullet()
    {
        AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
        int count = 10;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(ultimateBullet);
            xuanzhuanBullet1 bullet = obj.GetComponent<xuanzhuanBullet1>();
            bullet.player = transform;
            bullet.effectObj = damageEffect1;
            bullet.transform.parent = transform;
            bullet.target = c.transform;
            bullet.bulleting();
            yield return new WaitForSeconds(0.1f);
            if(i % 9 == 0)
            {
                
                c.attacked();
            }
        }
    }

    IEnumerator delayBullet1()
    {
        AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
        int count = 10;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(ultimateBullet);
            xuanzhuanBullet1 bullet = obj.GetComponent<xuanzhuanBullet1>();
            bullet.player = transform;
            bullet.effectObj = damageEffect1;
            bullet.transform.parent = transform;
            bullet.target = c.transform;
            bullet.flag = -1f;
            bullet.bulleting();
            yield return new WaitForSeconds(0.1f);
            if (i % 9 == 0)
            {

                c.attacked();
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
                if (damageEffect1 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect1);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                StartCoroutine(delayBullet3());
                break;
            case AnimationName1.Magic:
                if (magicBullet != null)
                {
                    GameObject obj = GameObject.Instantiate(magicBullet);
                    NormalBullet1 bullet = obj.GetComponent<NormalBullet1>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect2;
                    bullet.bulleting();


                }
                if (damageEffect2 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                StartCoroutine(delayBullet4());
                // StartCoroutine(delayBullet());
                // StartCoroutine(delayBullet1());
                break;
            case AnimationName1.Magic2:
                if (magic2Bullet != null)
                {
                    GameObject obj = GameObject.Instantiate(magic2Bullet);
                    NormalBullet1 bullet = obj.GetComponent<NormalBullet1>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect2;
                    bullet.bulleting();
                }
                if (damageEffect2 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                StartCoroutine(delayBullet5());
                break;
            case AnimationName1.Ultimate:
                if (damageEffect3 != null)
                {
                    GameObject obj1 = GameObject.Instantiate(damageEffect3);
                    ParticlesEffect1 effect = obj1.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                //StartCoroutine(delayBullet4());
                c.attacked();
                break;
        }
    }
}
