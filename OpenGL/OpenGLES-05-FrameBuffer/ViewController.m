//
//  ViewController.m
//  OpenGLES-05-Texture
//
//  Created by charles on 2020/7/30.
//  Copyright © 2020 charles. All rights reserved.
//

#import "ViewController.h"
#import "TFGLView.h"

@interface ViewController ()

@property (nonatomic, strong) TFGLView* mView;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    NSLog(@"view controller");
    self.mView = (TFGLView*)self.view;
    
}
@end
