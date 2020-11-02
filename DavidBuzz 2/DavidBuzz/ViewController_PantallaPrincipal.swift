//
//  ViewController_PantallaPrincipal.swift
//  DavidBuzz
//
//  Created by Fernando on 30/09/2020.
//  Copyright © 2020 Victor. All rights reserved.
//

import UIKit

class ViewController_PantallaPrincipal: UIViewController {
    @IBOutlet weak var puerto: UILabel!
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        
    }
    
    @IBAction func BotonDavid(_ sender: UIButton) {
    }
    
    @IBAction func BotonGuia(_ sender: UIButton) {
    }
    @IBAction func SeleccionPuerto(_ sender: UIStepper) {
        puerto.text = String(String(sender.value).dropLast().dropLast());
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.destination is ViewController_David {
            let aux = segue.destination as! ViewController_David
            aux.puerto = UInt16(self.puerto.text!)
        }
        if segue.destination is ViewController_Guia {
            let aux2 = segue.destination as! ViewController_Guia
            aux2.puerto = UInt16(self.puerto.text!)
        }
        
    }

}
