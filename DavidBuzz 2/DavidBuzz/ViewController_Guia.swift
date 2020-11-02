//
//  ViewController_Guia.swift
//  DavidBuzz
//
//  Created by Fernando on 01/10/2020.
//  Copyright © 2020 Victor. All rights reserved.
//

import UIKit

class ViewController_Guia: UIViewController {

    var puerto: UInt16?;
    var david: Cliente?
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        david =  Cliente(host: "91.121.118.108", port: self.puerto!);
        david!.start();
        david!.send(data: "0".data(using: .utf8)!);
        // Do any additional setup after loading the view.
    }
    
    @IBAction func ARRIBA_IZDA(_ sender: UIButton) {
        david?.send(data: "7".data(using: .utf8)!)
        
    }
    
    @IBAction func ARRIBA_DCHA(_ sender: UIButton) {
        david?.send(data: "9".data(using: .utf8)!)
    }
    
    @IBAction func ABAJO_IZDA(_ sender: UIButton) {
        david?.send(data: "1".data(using: .utf8)!)
    }
    @IBAction func ABAJO_DCHA(_ sender: UIButton) {
        david?.send(data: "3".data(using: .utf8)!)
    }
    
    @IBAction func TerminarConesion(_ sender: UIButton) {
        david!.stop()
    }
    
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
