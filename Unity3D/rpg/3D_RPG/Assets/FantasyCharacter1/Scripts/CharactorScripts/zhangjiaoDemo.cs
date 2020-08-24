using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class huatuoDemo : MonoBehaviour {

    public GameObject attackBullet;
    public GameObject magicBullet;
    public GameObject magic2Bullet;
    public GameObject ultimateBullet;
    public GameObject rbullet;
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
        AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
        List<RotateBullet1> list = new List<RotateBullet1>();
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(rbullet);
            RotateBullet1 bullet = obj.GetComponent<RotateBullet1>();
            bullet.player = c.transform;
            bullet.effectObj = damageEffect2;
            bullet.target = c.transform;
            bullet.bulleting();
            bullet.r = 2.7f;
            bullet.y = 1f;
            list.Add(bullet);
            yield return new WaitForSeconds(0.25f);
        }
        yield return new WaitForSeconds(1.5f);
        for (int i = 0; i < list.Count; i++)
        {
            list[i].bulletState = BulletState1.wand;
            list[i].tarPos = MathUtil1.findChild(c.transform, "attackedPivot").position;
            yield return null;
        }
        c.attacked();
    }

    IEnumerator delayBullet4()
    {
        int count = 12;
        AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
        List<RotateBullet1> list = new List<RotateBullet1>();
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(rbullet);
            RotateBullet1 bullet = obj.GetComponent<RotateBullet1>();
            bullet.player = c.transform;
            bullet.effectObj = damageEffect1;
            bullet.target = c.transform;
            bullet.bulleting();
            bullet.r = 2.7f;
            bullet.y = 0.5f;
            list.Add(bullet);
            yield return new WaitForSeconds(0.25f);
        }
        yield return new WaitForSeconds(1.5f);
        for (int i = 0; i < list.Count; i++)
        {
            list[i].bulletState = BulletState1.wand;
            list[i].tarPos = MathUtil1.findChild(c.transform, "attackedPivot").position;
            yield return null;
        }
        c.attacked();
    }

    IEnumerator delayBullet5()
    {
        int count = 12;
        AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
        List<RotateBullet1> list = new List<RotateBullet1>();
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(rbullet);
            RotateBullet1 bullet = obj.GetComponent<RotateBullet1>();
            bullet.player = c.transform;
            bullet.effectObj = damageEffect3;
            bullet.target = c.transform;
            bullet.bulleting();
            bullet.r = 2.7f;
            bullet.y = 1.5f;
            list.Add(bullet);
            yield return new WaitForSeconds(0.25f);
        }
        yield return new WaitForSeconds(1.5f);
        for (int i = 0; i < list.Count; i++)
        {
            list[i].bulletState = BulletState1.wand;
            list[i].tarPos = MathUtil1.findChild(c.transform, "attackedPivot").position;
            yield return null;
        }
        c.attacked();
    }

    IEnumerator delayBullet()
    {
        int count = 10;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(rbullet);
            RotateBullet bullet = obj.GetComponent<RotateBullet>();
            bullet.player = transform;
            bullet.effectObj = damageEffect1;
            bullet.bulleting();
            bullet.y = 0.35f;
            yield return new WaitForSeconds(0.1f);
            if (i % 9 == 0)
            {
                AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
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
            GameObject obj = GameObject.Instantiate(rbullet);
            RotateBullet bullet = obj.GetComponent<RotateBullet>();
            bullet.player = transform;
            bullet.effectObj = damageEffect1;
            bullet.bulleting();
            bullet.y = 0.7f;
            bullet.flag = -1f;
            yield return new WaitForSeconds(0.1f);
            if (i % 9 == 0)
            {
                AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
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
        string[] arr = actionName.Split('|');
        string name = arr[0];
        switch(name)
        {
            case AnimationName1.Attack:
                if(attackBullet != null)
                {
                    GameObject obj = GameObject.Instantiate(attackBullet);
                    NormalBullet bullet = obj.GetComponent<NormalBullet>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect1;
                    bullet.bulleting();
                }
                StartCoroutine(delayBullet3());
                break;
            case AnimationName1.Magic:
                if (magicBullet != null)
                {
                    GameObject obj = GameObject.Instantiate(magicBullet);
                    NormalBullet bullet = obj.GetComponent<NormalBullet>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect1;
                    bullet.bulleting();
                }
                StartCoroutine(delayBullet4());
                break;
            case AnimationName1.Magic2:
                if (magic2Bullet != null)
                {
                    GameObject obj = GameObject.Instantiate(magic2Bullet);
                    NormalBullet bullet = obj.GetComponent<NormalBullet>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect2;
                    bullet.bulleting();
                }
                StartCoroutine(delayBullet5());
                break;
            case AnimationName1.Ultimate:

               
                break;
        }
    }
}
