//
//  ViewController_David.swift
//  DavidBuzz
//
//  Created by Fernando on 30/09/2020.
//  Copyright © 2020 Victor. All rights reserved.
//

import UIKit



class ViewController_David: UIViewController {

    var puerto: UInt16?;
    var david: Cliente?
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        david =  Cliente(host: "91.121.118.108", port: self.puerto!);
        david!.start();
        david!.send(data: "1".data(using: .utf8)!);
        // Do any additional setup after loading the view.
    }
    
    @IBAction func TerminarConexion(_ sender: UIButton) {
        david?.stop()
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
