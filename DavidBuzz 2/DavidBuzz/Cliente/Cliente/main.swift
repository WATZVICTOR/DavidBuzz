//
//  main.swift
//  Cliente
//
//  Created by Fernando on 30/09/2020.
//  Copyright © 2020 Victor. All rights reserved.
//

import Foundation

var cl = Cliente(host: "91.121.118.108", port: UInt16("10023")!);
cl.start();
while(true) {
    let cmd = readLine();
    cl.send(data: cmd!.data(using: .utf8)!)
}
RunLoop.current.run()

